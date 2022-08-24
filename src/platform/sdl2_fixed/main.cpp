#include <SDL.h>

#include "game.h"

#define WND_TITLE    "OpenLara"

SDL_Window* sdl_window;
SDL_DisplayMode sdl_displaymode;

int FRAME_WIDTH = 1024; // 1280;
int FRAME_HEIGHT = 768; // 720;

int32 fps;
int32 frameIndex = 0;
int32 fpsCounter = 0;
uint32 curSoundBuffer = 0;

const void* TRACKS_IMA;
const void* TITLE_SCR;
const void* levelData;

//HWND hWnd;
//HDC hDC;

int32 g_timer;
//LARGE_INTEGER g_current;

//LARGE_INTEGER gTimerFreq;
//LARGE_INTEGER gTimerStart;

bool isQuit = false;

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


void osJoyVibrate(int32 index, int32 L, int32 R)
{
}


void inputInit() {
#if 0
    memset(joyDevice, 0, sizeof(joyDevice));

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
    }
#endif
}

void inputFree()
{
//    memset(joyDevice, 0, sizeof(joyDevice));
}

void inputUpdate()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
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

//uint8 soundBuffer[2 * SND_SAMPLES + 32]; // 32 bytes of silence for DMA overrun while interrupt

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

#if 0
LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_ACTIVATE:
        {
            keys = 0;
            break;
        }

        case WM_DESTROY :
        {
            PostQuitMessage(0);
            break;
        }

        case WM_KEYDOWN    :
        case WM_KEYUP      :
        case WM_SYSKEYUP   :
        case WM_SYSKEYDOWN :
        {
            InputKey key = IK_NONE;
            switch (wParam) {
                case VK_UP     : key = IK_UP;     break;
                case VK_RIGHT  : key = IK_RIGHT;  break;
                case VK_DOWN   : key = IK_DOWN;   break;
                case VK_LEFT   : key = IK_LEFT;   break;
                case 'A'       : key = IK_B;      break;
                case 'S'       : key = IK_A;      break;
                case 'Q'       : key = IK_L;      break;
                case 'W'       : key = IK_R;      break;
                case VK_RETURN : key = IK_START;  break;
                case VK_SPACE  : key = IK_SELECT; break;
            }

            if (wParam == '1') players[0]->extraL->goalWeapon = WEAPON_PISTOLS;
            if (wParam == '2') players[0]->extraL->goalWeapon = WEAPON_MAGNUMS;
            if (wParam == '3') players[0]->extraL->goalWeapon = WEAPON_UZIS;
            if (wParam == '4') players[0]->extraL->goalWeapon = WEAPON_SHOTGUN;

            if (msg != WM_KEYUP && msg != WM_SYSKEYUP) {
                keys |= key;
            } else {
                keys &= ~key;
            }
            break;
        }

        case MM_WOM_DONE :
        {
            soundFill();
            break;
        }

        case WM_SIZE :
        {
            FRAME_WIDTH = LOWORD(lParam);
            FRAME_HEIGHT = HIWORD(lParam);
            break;
        }

        default :
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
#endif

const void* osLoadScreen(LevelID id)
{
    return (const void*)1; // TODO
}

const void* osLoadLevel(LevelID id)
{
    // level1
    char buf[32];

    delete[] levelData;

    sprintf(buf, "data/%s.PHD", (char*)gLevelInfo[id].data);

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

// hint to the driver to use discrete GPU
#if 0
extern "C" {
    __declspec(dllexport) int NvOptimusEnablement = 1; // NVIDIA
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; // AMD
}
#endif

int main(int argc, char* argv[])
{
    SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER);

    SDL_GetCurrentDisplayMode(0, &sdl_displaymode);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    sdl_window = SDL_CreateWindow(WND_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        FRAME_WIDTH, FRAME_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    // We try to use the current video mode, but we inform the core of whatever mode SDL2 gave us in the end.
    //SDL_GetWindowSize(sdl_window, &w, &h);

    SDL_GLContext context = SDL_GL_CreateContext(sdl_window);

    gameInit();

//    Core::width = w;
    //Core::height = h;


    //SDL_ShowCursor(SDL_DISABLE);

    int32 startTime = osGetSystemTimeMS() - 33;
    int32 lastFrame = 0;

    do {
        inputUpdate();

        int32 frame = (osGetSystemTimeMS() - startTime) / 33;
       // if (GetAsyncKeyState('R')) frame /= 10;

        int32 count = frame - lastFrame;
        //if (GetAsyncKeyState('T')) count *= 10;
        gameUpdate(count);
        lastFrame = frame;

        gameRender();

        renderSwap();
    } while (!isQuit); // msg.message != WM_QUIT);

    gameFree();

    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

    return 0;
}

#if 0
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    QueryPerformanceFrequency(&gTimerFreq);
    QueryPerformanceCounter(&gTimerStart);

//    int argc = (lpCmdLine && strlen(lpCmdLine)) ? 2 : 1;
//    const char* argv[] = { "", lpCmdLine };

    RECT r = { 0, 0, FRAME_WIDTH, FRAME_HEIGHT };

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    if (sw <= r.right + 128 || sh <= r.bottom + 128) {
        r.right /= 2;
        r.bottom /= 2;
    }

    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
    int wx = (sw - (r.right - r.left)) / 2;
    int wy = (sh - (r.bottom - r.top)) / 2;

    hWnd = CreateWindow("static", "OpenLara", WS_OVERLAPPEDWINDOW, wx + r.left, wy + r.top, r.right - r.left, r.bottom - r.top, 0, 0, 0, 0);
    hDC = GetDC(hWnd);

    SetWindowLong(hWnd, GWL_WNDPROC, (LONG)&wndProc);
    ShowWindow(hWnd, SW_SHOWDEFAULT);

    soundInit();
    inputInit();

    gameInit();

    MSG msg;

    int32 startTime = GetTickCount() - 33;
    int32 lastFrame = 0;

    do {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
        #ifdef _DEBUG
            Sleep(4);
        #endif
            inputUpdate();

            int32 frame = (GetTickCount() - startTime) / 33;
            if (GetAsyncKeyState('R')) frame /= 10;

            int32 count = frame - lastFrame;
            if (GetAsyncKeyState('T')) count *= 10;
            gameUpdate(count);
            lastFrame = frame;

            gameRender();

            renderSwap();
        }
    } while (msg.message != WM_QUIT);

    inputFree();
    gameFree();

    return 0;
}
#endif
