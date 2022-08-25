#include <SDL.h>

#include "game.h"

int FRAME_WIDTH = 1024; //1280;
int FRAME_HEIGHT = 768; //720;

SDL_Surface *screen = NULL;
bool isQuit = false;

int32 fps;
int32 frameIndex = 0;
int32 fpsCounter = 0;
uint32 curSoundBuffer = 0;

const void* TRACKS_IMA;
const void* TITLE_SCR;
const void* levelData;

void osSetPalette(const uint16* palette)
{
    //
}

int32 osGetSystemTimeMS()
{
	return SDL_GetTicks();
}

bool osSaveSettings()
{
    FILE* f = fopen("settings.dat", "wb");
    if (!f) return false;
    fwrite(&gSettings, sizeof(gSettings), 1, f);
    fclose(f);
    return true;
}

bool osLoadSettings()
{
    FILE* f = fopen("settings.dat", "rb");
    if (!f) return false;
    uint8 version;
    fread(&version, 1, 1, f);
    if (version != gSettings.version) {
        fclose(f);
        return false;
    }
    fread((uint8*)&gSettings + 1, sizeof(gSettings) - 1, 1, f);
    fclose(f);
    return true;
}

bool osCheckSave()
{
    FILE* f = fopen("savegame.dat", "rb");
    if (!f) return false;
    fclose(f);
    return true;
}

bool osSaveGame()
{
    FILE* f = fopen("savegame.dat", "wb");
    if (!f) return false;
    fwrite(&gSaveGame, sizeof(gSaveGame), 1, f);
    fwrite(&gSaveData, gSaveGame.dataSize, 1, f);
    fclose(f);
    return true;
}

bool osLoadGame()
{
    FILE* f = fopen("savegame.dat", "rb");
    if (!f) return false;

    uint32 version;
    fread(&version, sizeof(version), 1, f);

    if (SAVEGAME_VER != version)
    {
        fclose(f);
        return false;
    }

    fread(&gSaveGame.dataSize, sizeof(gSaveGame) - sizeof(version), 1, f);
    fread(&gSaveData, gSaveGame.dataSize, 1, f);
    fclose(f);
    return true;
}

void inputInit() {
/*    memset(joyDevice, 0, sizeof(joyDevice));

    HMODULE h = LoadLibrary("xinput1_3.dll");
    if (h == NULL) {
        h = LoadLibrary("xinput9_1_0.dll");
    }

    if (!h)
        return;

    #define GetProcAddr(lib, x) (x = (decltype(x))GetProcAddress(lib, #x + 1))

    GetProcAddr(h, _XInputGetState);
    GetProcAddr(h, _XInputSetState);
    GetProcAddr(h, _XInputEnable);

    for (int i = 0; i < INPUT_JOY_COUNT; i++)
    {
        XINPUT_STATE state;
        int res = _XInputGetState(i, &state);
        joyDevice[i].ready = (_XInputGetState(i, &state) == ERROR_SUCCESS);

        if (joyDevice[i].ready)
            LOG("Gamepad %d is ready\n", i + 1);
    }*/
}

void inputFree()
{
  //  memset(joyDevice, 0, sizeof(joyDevice));
}

void inputUpdate()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		uint32 key = 0;

		switch(event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:	isQuit = true;	break;
					case SDLK_UP:	key = IK_UP;	break;
					case SDLK_DOWN:	key = IK_DOWN;	break;
					case SDLK_LEFT:	key = IK_LEFT;	break;
					case SDLK_RIGHT:	key = IK_RIGHT;	break;
					case SDLK_a:	key = IK_B;      break;
					case SDLK_s:	key = IK_A;      break;
					case SDLK_q:	key = IK_L;      break;
					case SDLK_w:	key = IK_R;      break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
						key = IK_START;	break;
					case SDLK_SPACE:	key = IK_SELECT;	break;
					case SDLK_1:
					case SDLK_KP1:
						players[0]->extraL->goalWeapon = WEAPON_PISTOLS;
						break;
					case SDLK_2:
					case SDLK_KP2:
						players[0]->extraL->goalWeapon = WEAPON_MAGNUMS;
						break;
					case SDLK_3:
					case SDLK_KP3:
						players[0]->extraL->goalWeapon = WEAPON_UZIS;
						break;
					case SDLK_4:
					case SDLK_KP4:
						players[0]->extraL->goalWeapon = WEAPON_SHOTGUN;
						break;
					default:	break;
				}

				keys |= key;
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
					case SDLK_UP:	key = IK_UP;	break;
					case SDLK_DOWN:	key = IK_DOWN;	break;
					case SDLK_LEFT:	key = IK_LEFT;	break;
					case SDLK_RIGHT:	key = IK_RIGHT;	break;
					case SDLK_a:	key = IK_B;      break;
					case SDLK_s:	key = IK_A;      break;
					case SDLK_q:	key = IK_L;      break;
					case SDLK_w:	key = IK_R;      break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
						key = IK_START;	break;
					case SDLK_SPACE:	key = IK_SELECT;	break;
					default:	break;
				}

				keys &= ~key;
				break;
			case SDL_VIDEORESIZE:
				FRAME_WIDTH = event.resize.w;
				FRAME_HEIGHT = event.resize.h;
				break;
			case SDL_QUIT:
				isQuit = true;
				break;
            default:
                break;
		}
	}


#if 0
	if (!_XInputGetState)
        return;

    for (int i = 0; i < INPUT_JOY_COUNT; i++)
    {
        if (!joyDevice[i].ready)
            continue;

        joyRumble(i);

        XINPUT_STATE state;
        if (_XInputGetState(i, &state) != ERROR_SUCCESS)
        {
            inputFree();
            inputInit();
            break;
        }

        static const InputKey buttons[] = { IK_UP, IK_DOWN, IK_LEFT, IK_RIGHT, IK_START, IK_SELECT, IK_NONE, IK_NONE, IK_L, IK_R, IK_NONE, IK_NONE, IK_A, IK_B, IK_X, IK_Y };

        int32 curMask = state.Gamepad.wButtons;
        int32 oldMask = joyDevice[i].mask;

        for (int i = 0; i < 16; i++)
        {
            bool wasDown = (oldMask & (1 << i)) != 0;
            bool isDown = (curMask & (1 << i)) != 0;

            if (isDown == wasDown)
                continue;

            if (isDown && !wasDown) {
                keys |= buttons[i];
            } else {
                keys &= ~buttons[i];
            }
        }

        joyDevice[i].mask = curMask;


        //osJoyVibrate(j, state.Gamepad.bLeftTrigger / 255.0f, state.Gamepad.bRightTrigger / 255.0f); // vibration test

        /*
        Input::setJoyPos(j, jkL, joyDir(joyAxis(state.Gamepad.sThumbLX, -32768, 32767),
            joyAxis(-state.Gamepad.sThumbLY, -32768, 32767)));
        Input::setJoyPos(j, jkR, joyDir(joyAxis(state.Gamepad.sThumbRX, -32768, 32767),
            joyAxis(-state.Gamepad.sThumbRY, -32768, 32767)));
        Input::setJoyPos(j, jkLT, vec2(state.Gamepad.bLeftTrigger / 255.0f, 0.0f));
        Input::setJoyPos(j, jkRT, vec2(state.Gamepad.bRightTrigger / 255.0f, 0.0f));
        */




    }

#endif
}

void osJoyVibrate(int32 index, int32 L, int32 R)
{
//    joyDevice[index].vL = L;
//    joyDevice[index].vR = R;
}

uint8 soundBuffer[2 * SND_SAMPLES + 32]; // 32 bytes of silence for DMA overrun while interrupt

//HWAVEOUT waveOut;
//WAVEFORMATEX waveFmt = { WAVE_FORMAT_PCM, 1, SND_OUTPUT_FREQ, SND_OUTPUT_FREQ, 1, 8, sizeof(waveFmt) };
//WAVEHDR waveBuf[2];

void soundInit()
{
#if 0
    sndInit();

    if (waveOutOpen(&waveOut, WAVE_MAPPER, &waveFmt, (INT_PTR)hWnd, 0, CALLBACK_WINDOW) != MMSYSERR_NOERROR)
        return;

    memset(&waveBuf, 0, sizeof(waveBuf));
    for (int i = 0; i < 2; i++)
    {
        WAVEHDR *waveHdr = waveBuf + i;
        waveHdr->dwBufferLength = SND_SAMPLES;
        waveHdr->lpData = (LPSTR)(soundBuffer + i * SND_SAMPLES);
        waveOutPrepareHeader(waveOut, waveHdr, sizeof(WAVEHDR));
        waveOutWrite(waveOut, waveHdr, sizeof(WAVEHDR));
    }
#endif
}

void soundFill()
{
#if 0
    WAVEHDR *waveHdr = waveBuf + curSoundBuffer;
    waveOutUnprepareHeader(waveOut, waveHdr, sizeof(WAVEHDR));
    sndFill((uint8*)waveHdr->lpData, SND_SAMPLES);
    waveOutPrepareHeader(waveOut, waveHdr, sizeof(WAVEHDR));
    waveOutWrite(waveOut, waveHdr, sizeof(WAVEHDR));
    curSoundBuffer ^= 1;
#endif
}

const void* osLoadScreen(LevelID id)
{
    return (const void*)1; // TODO
}

const void* osLoadLevel(LevelID id)
{
    // level1
    char buf[32];

    delete[] levelData;

    sprintf(buf, "DATA/%s.PHD", (char*)gLevelInfo[id].data);

    FILE *f = fopen(buf, "rb");

    if (!f) {
        LOG("level file not found!");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    int32 size = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8* data = new uint8[size];
    fread(data, 1, size, f);
    fclose(f);

    levelData = data;

    return (void*)levelData;
}

int main(int argc, char **argv)
{
//    int argc = (lpCmdLine && strlen(lpCmdLine)) ? 2 : 1;
//    const char* argv[] = { "", lpCmdLine };

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE |SDL_INIT_JOYSTICK | SDL_OPENGL);
    SDL_ShowCursor(0);

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);

	screen = SDL_SetVideoMode(FRAME_WIDTH, FRAME_HEIGHT, 32, SDL_DOUBLEBUF | SDL_WINDOW_RESIZABLE);

    soundInit();
    inputInit();

    gameInit();

    int32 startTime = osGetSystemTimeMS() - 33;
    int32 lastFrame = 0;

    do {
		inputUpdate();

		int32 frame = (osGetSystemTimeMS() - startTime) / 33;
		//if (GetAsyncKeyState('R')) frame /= 10;

		int32 count = frame - lastFrame;
		//if (GetAsyncKeyState('T')) count *= 10;
		gameUpdate(count);
		lastFrame = frame;

		gameRender();

		renderSwap();
    } while (!isQuit);

    inputFree();
    gameFree();

	SDL_Quit();

    return 0;
}
