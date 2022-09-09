#include "jkGUINetHost.h"

#include <errno.h>

#include "General/stdBitmap.h"
#include "General/stdFont.h"
#include "Engine/rdMaterial.h" // TODO move stdVBuffer
#include "Engine/sithMulti.h"
#include "stdPlatform.h"
#include "jk.h"
#include "Gui/jkGUIRend.h"
#include "Gui/jkGUI.h"
#include "Gui/jkGUIDialog.h"
#include "Gui/jkGUINet.h"
#include "Main/jkStrings.h"
#include "Main/jkMain.h"
#include "Win95/sithDplay.h"
#include "Platform/wuRegistry.h"
#include "General/stdString.h"
#include "Main/jkRes.h"
#include "Main/jkEpisode.h"
#include "Gui/jkGUISingleplayer.h"

static int jkGuiNetHost_aIdk[2] = {0xd, 0xe};

static jkGuiElement jkGuiNetHost_aElements[28] =
{
    { ELEMENT_TEXT, 0, 0, NULL, 3, { 0, 410, 640, 20 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXT, 0, 6, "GUI_MULTIPLAYER", 3, { 20, 20, 600, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXT, 0, 0, "GUI_GAMENAME", 2, { 20, 80, 270, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXTBOX, 0, 0, NULL, 16, { 20, 125, 270, 20 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXT, 0, 0, "GUI_MAXPLAYERS", 2, { 20, 155, 200, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXTBOX, 0, 0, NULL, 2, { 225, 160, 65, 20 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_CHECKBOX, 0, 0, "GUI_SCORELIMIT", 0, { 20, 190, 200, 40 }, 1, 0, "GUI_SCORELIMIT_HINT", NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXTBOX, 0, 0, NULL, 3, { 225, 195, 65, 20 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_CHECKBOX, 0, 0, "GUI_TIMELIMIT", 0, { 20, 225, 200, 40 }, 1, 0, "GUI_TIMELIMIT_HINT", NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXTBOX, 0, 0, NULL, 3, { 225, 230, 65, 20 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_CHECKBOX, 0, 0, "GUI_TEAMMODE", 0, { 20, 260, 200, 40 }, 1, 0, "GUI_TEAMMODE_HINT", NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_CHECKBOX, 0, 0, "GUI_SINGLELEVEL", 0, { 20, 295, 200, 40 }, 1, 0, "GUI_SINGLELEVEL_HINT", NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXT, 0, 0, "GUI_MAXSTARS", 2, { 310, 80, 150, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXT, 0, 0, NULL, 2, { 375, 120, 120, 30 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_PICBUTTON, 1, 0, NULL, 33, { 310, 120, 30, 30 }, 1, 0, NULL, NULL, &jkGuiNetHost_sub_411AE0, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_PICBUTTON, 2, 0, NULL, 34, { 340, 120, 30, 30 }, 1, 0, NULL, NULL, &jkGuiNetHost_sub_411AE0, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXT, 0, 0, "GUINET_PASSWORD", 2, { 20, 340, 270, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXTBOX, 0, 0, NULL, 16, { 20, 380, 270, 20 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXT, 0, 0, "GUI_CHOOSEEPISODE", 2, { 310, 150, 240, 20 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_LISTBOX, 0, 0, NULL, 0, { 310, 175, 320, 80 }, 1, 0, NULL, NULL, NULL, jkGuiNetHost_aIdk, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXT, 0, 0, "GUI_CHOOSELEVEL", 2, { 310, 265, 260, 20 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_LISTBOX, 1, 0, NULL, 0, { 310, 290, 320, 110 }, 1, 0, NULL, NULL, NULL, jkGuiNetHost_aIdk, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXTBUTTON, 200, 2, "GUI_ADVANCED", 3, { 220, 430, 200, 40 }, 1, 0, "GUI_ADVANCED_HINT", NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXTBUTTON, 1, 2, "GUI_OK", 3, { 420, 430, 200, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXTBUTTON, -1, 2, "GUI_CANCEL", 3, { 20, 430, 200, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
#ifdef QOL_IMPROVEMENTS
    { ELEMENT_TEXT, 0, 0, L"Server Port:", 2, { 540, 80, 90, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXTBOX, 0, 0, NULL, 16, { 540, 125, 90, 20 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    
#endif
    { ELEMENT_END, 0, 0, NULL, 0, { 0, 0, 0, 0 }, 0, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 }
};

static jkGuiMenu jkGuiNetHost_menu =
{
    &jkGuiNetHost_aElements, 0, 65535, 65535, 15, NULL, NULL, jkGui_stdBitmaps, jkGui_stdFonts, 0, NULL, "thermloop01.wav", "thrmlpu2.wav", NULL, NULL, NULL, 0, NULL, NULL
};

static jkGuiElement jkGuiNetHost_aSettingsElements[9] =
{
    { ELEMENT_TEXT, 0, 0, NULL, 3, { 0, 410, 640, 20 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXT, 0, 6, "GUI_MULTIPLAYER", 3, { 20, 20, 600, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXT, 0, 0, "GUI_TICKRATE", 2, { 70, 230, 200, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXTBOX, 0, 0, NULL, 0, { 280, 240, 50, 20 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
#ifdef QOL_IMPROVEMENTS
    { ELEMENT_CHECKBOX, 0, 0, L"Dedicated Server", 0, { 70, 270, 200, 40 }, 1, 0, L"Launch server without participating as a player.", NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_CHECKBOX, 0, 0, L"Experimental Co-op", 0, { 70, 300, 200, 40 }, 1, 0, L"Launch server with actors enabled.", NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
#endif
    { ELEMENT_TEXTBUTTON, 1, 2, "GUI_OK", 3, { 420, 430, 200, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_TEXTBUTTON, -1, 2, "GUI_CANCEL", 3, { 20, 430, 200, 40 }, 1, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 },
    { ELEMENT_END, 0, 0, NULL, 0, { 0, 0, 0, 0 }, 0, 0, NULL, NULL, NULL, NULL, { 0, 0, 0, 0, 0, { 0, 0, 0, 0 } }, 0 }
};

static jkGuiMenu jkGuiNetHost_menuSettings =
{
    &jkGuiNetHost_aSettingsElements, 0, 65535, 65535, 15, NULL, NULL, jkGui_stdBitmaps, jkGui_stdFonts, 0, NULL, "thermloop01.wav", "thrmlpu2.wav", NULL, NULL, NULL, 0, NULL, NULL
};

static int jkGuiNetHost_bInitted;
static wchar_t jkGuiNetHost_waIdk[32];
static Darray jkGuiNetHost_dArray1;
static Darray jkGuiNetHost_dArray2;

// Added
wchar_t jkGuiNetHost_portText[32];
int jkGuiNetHost_portNum = 27020;
int jkGuiNetHost_bIsDedicated = 0;
int jkGuiNetHost_bIsCoop = 0;

#define LONG_MAX ((long)(~0UL>>1))
#define LONG_MIN (~LONG_MAX)

long jk_wcstol(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base)
{
    const wchar_t *p = nptr, *endp;
    _Bool is_neg = 0, overflow = 0;
    /* Need unsigned so (-LONG_MIN) can fit in these: */
    unsigned long n = 0UL, cutoff;
    int cutlim;
    if (base < 0 || base == 1 || base > 36) {
#ifdef EINVAL /* errno value defined by POSIX */
        errno = EINVAL;
#endif
        return 0L;
    }
    endp = nptr;
    while (isspace(*p))
        p++;
    if (*p == '+') {
        p++;
    } else if (*p == '-') {
        is_neg = 1, p++;
    }
    if (*p == '0') {
        p++;
        /* For strtol(" 0xZ", &endptr, 16), endptr should point to 'x';
         * pointing to ' ' or '0' is non-compliant.
         * (Many implementations do this wrong.) */
        endp = p;
        if (base == 16 && (*p == 'X' || *p == 'x')) {
            p++;
        } else if (base == 0) {
            if (*p == 'X' || *p == 'x') {
                base = 16, p++;
            } else {
                base = 8;
            }
        }
    } else if (base == 0) {
        base = 10;
    }
    cutoff = (is_neg) ? -(LONG_MIN / base) : LONG_MAX / base;
    cutlim = (is_neg) ? -(LONG_MIN % base) : LONG_MAX % base;
    while (1) {
        int c;
        if (*p >= 'A')
            c = ((*p - 'A') & (~('a' ^ 'A'))) + 10;
        else if (*p <= '9')
            c = *p - '0';
        else
            break;
        if (c < 0 || c >= base) break;
        endp = ++p;
        if (overflow) {
            /* endptr should go forward and point to the non-digit character
             * (of the given base); required by ANSI standard. */
            if (endptr) continue;
            break;
        }
        if (n > cutoff || (n == cutoff && c > cutlim)) {
            overflow = 1; continue;
        }
        n = n * base + c;
    }
    if (endptr) *endptr = (wchar_t *)endp;
    if (overflow) {
        errno = ERANGE; return ((is_neg) ? LONG_MIN : LONG_MAX);
    }
    return (long)((is_neg) ? -n : n);
}

int msvc_sub_5133E0(wchar_t *a1, wchar_t **a2, char a3)
{
    //TODO
    return jk_wcstol(a1, a2, a3);
}

int wstr_to_int_clamped(wchar_t *pWstr, int minVal, int maxVal)
{
    wchar_t *dummy;
    int val = msvc_sub_5133E0(pWstr, &dummy, 10);
     if (val < minVal)
    {
        return minVal;
    }
    else if (val > maxVal)
    {
        return maxVal;
    }

    return val;
}

// Added: Make sure stuff actually gets into the registry
void jkGuiNetHost_SaveSettings()
{
#ifdef QOL_IMPROVEMENTS
    if (jkGuiNetHost_bIsDedicated) {
        jkGuiNetHost_maxPlayers -= 1;
    }
    if (jkGuiNetHost_bIsDedicated) {
        jkGuiNetHost_sessionFlags |= SESSIONFLAG_ISDEDICATED;
    }
    else {
        jkGuiNetHost_sessionFlags &= ~SESSIONFLAG_ISDEDICATED;
    }

    if (jkGuiNetHost_bIsDedicated) {
        jkGuiNetHost_gameFlags |= MULTIMODEFLAG_COOP;
    }
    else {
        jkGuiNetHost_gameFlags &= ~MULTIMODEFLAG_COOP;
    }

#endif
    wuRegistry_SaveInt("maxRank", jkGuiNetHost_maxRank);
    wuRegistry_SaveInt("sessionFlags", jkGuiNetHost_sessionFlags);
    wuRegistry_SaveInt("gameFlags", jkGuiNetHost_gameFlags);
    wuRegistry_SaveInt("timeLimit", jkGuiNetHost_timeLimit);
    wuRegistry_SaveInt("scoreLimit", jkGuiNetHost_scoreLimit);
    wuRegistry_SaveInt("maxPlayers", jkGuiNetHost_maxPlayers);
    wuRegistry_SaveInt("tickRate", jkGuiNetHost_tickRate);
#ifndef ARCH_WASM
    wuRegistry_SetWString("gameName", jkGuiNetHost_gameName);
#endif
#ifdef QOL_IMPROVEMENTS
    wuRegistry_SaveInt("portNum", jkGuiNetHost_portNum);
    wuRegistry_SaveBool("bIsDedicated", jkGuiNetHost_bIsDedicated);
    wuRegistry_SaveBool("bIsCoop", jkGuiNetHost_bIsCoop);

    wuRegistry_SaveBool("bUseScoreLimit", jkGuiNetHost_gameFlags & MULTIMODEFLAG_SCORELIMIT);
    wuRegistry_SaveBool("bUseTimeLimit", jkGuiNetHost_gameFlags & MULTIMODEFLAG_TIMELIMIT);
    wuRegistry_SaveBool("bIsSingleLevel", jkGuiNetHost_gameFlags & MULTIMODEFLAG_SINGLE_LEVEL);
    wuRegistry_SaveBool("bIsTeams", jkGuiNetHost_gameFlags & MULTIMODEFLAG_TEAMS);
#endif
}

void jkGuiNetHost_LoadSettings()
{
    jkGuiNetHost_maxRank = wuRegistry_GetInt("maxRank", jkGuiNetHost_maxRank);
    jkGuiNetHost_sessionFlags = wuRegistry_GetInt("sessionFlags", jkGuiNetHost_sessionFlags);
    jkGuiNetHost_gameFlags = wuRegistry_GetInt("gameFlags", jkGuiNetHost_gameFlags);
    jkGuiNetHost_timeLimit = wuRegistry_GetInt("timeLimit", jkGuiNetHost_timeLimit);
    jkGuiNetHost_scoreLimit = wuRegistry_GetInt("scoreLimit", jkGuiNetHost_scoreLimit);
    jkGuiNetHost_maxPlayers = wuRegistry_GetInt("maxPlayers", jkGuiNetHost_maxPlayers);
    jkGuiNetHost_tickRate = wuRegistry_GetInt("tickRate", jkGuiNetHost_tickRate);
    memset(jkGuiNetHost_gameName, 0, sizeof(jkGuiNetHost_gameName));
#ifndef ARCH_WASM
    wuRegistry_GetWString("gameName", jkGuiNetHost_gameName, 0x40u, jkGuiNetHost_gameName);
#endif

#ifdef QOL_IMPROVEMENTS
    jkGuiNetHost_portNum = wuRegistry_GetInt("portNum", jkGuiNetHost_portNum);
    jkGuiNetHost_bIsDedicated = wuRegistry_GetBool("bIsDedicated", jkGuiNetHost_bIsDedicated);

    if (jkGuiNetHost_bIsDedicated) {
        jkGuiNetHost_maxPlayers += 1;
    }

    if (jkGuiNetHost_bIsDedicated) {
        jkGuiNetHost_sessionFlags |= SESSIONFLAG_ISDEDICATED;
    }
    else {
        jkGuiNetHost_sessionFlags &= ~SESSIONFLAG_ISDEDICATED;
    }

    jkGuiNetHost_bIsCoop = wuRegistry_GetBool("bIsCoop", jkGuiNetHost_bIsCoop);
    if (jkGuiNetHost_bIsCoop) {
        jkGuiNetHost_gameFlags |= MULTIMODEFLAG_COOP;
    }
    else {
        jkGuiNetHost_gameFlags &= ~MULTIMODEFLAG_COOP;
    }

    if(wuRegistry_GetBool("bUseScoreLimit", jkGuiNetHost_gameFlags & MULTIMODEFLAG_SCORELIMIT)) {
        jkGuiNetHost_gameFlags |= MULTIMODEFLAG_SCORELIMIT;
    }
    else {
        jkGuiNetHost_gameFlags &= ~MULTIMODEFLAG_SCORELIMIT;
    }

    if(wuRegistry_GetBool("bUseTimeLimit", jkGuiNetHost_gameFlags & MULTIMODEFLAG_TIMELIMIT)) {
        jkGuiNetHost_gameFlags |= MULTIMODEFLAG_TIMELIMIT;
    }
    else {
        jkGuiNetHost_gameFlags &= ~MULTIMODEFLAG_TIMELIMIT;
    }

    if(wuRegistry_GetBool("bIsSingleLevel", jkGuiNetHost_gameFlags & MULTIMODEFLAG_SINGLE_LEVEL)) {
        jkGuiNetHost_gameFlags |= MULTIMODEFLAG_SINGLE_LEVEL;
    }
    else {
        jkGuiNetHost_gameFlags &= ~MULTIMODEFLAG_SINGLE_LEVEL;
    }

    if(wuRegistry_GetBool("bIsTeams", jkGuiNetHost_gameFlags & MULTIMODEFLAG_TEAMS)) {
        jkGuiNetHost_gameFlags |= MULTIMODEFLAG_TEAMS;
    }
    else {
        jkGuiNetHost_gameFlags &= ~MULTIMODEFLAG_TEAMS;
    }

#endif
}

void jkGuiNetHost_Initialize()
{
    jkGui_InitMenu(&jkGuiNetHost_menu, jkGui_stdBitmaps[2]);
    jkGui_InitMenu(&jkGuiNetHost_menuSettings, jkGui_stdBitmaps[3]);
    
    jkGuiNetHost_LoadSettings();
    jkGuiNetHost_bInitted = 1;
}

void jkGuiNetHost_Shutdown()
{
    jkGuiNetHost_SaveSettings();
    jkGuiNetHost_bInitted = 0;
}

int jkGuiNetHost_Show(jkMultiEntry3 *pMultiEntry)
{
    wchar_t *v1; // eax
    wchar_t *v2; // eax
    wchar_t *v3; // eax
    int v4; // ebp
    wchar_t *i; // eax
    int v10; // eax
    __int64 v11; // rax
    int v12; // eax
    int v13; // eax
    int v14; // eax
    int v15; // eax
    int v20; // [esp-8h] [ebp-188h]
    wchar_t *v21; // [esp-4h] [ebp-184h]
    int v23; // [esp+14h] [ebp-16Ch]
    wchar_t *a2; // [esp+18h] [ebp-168h] BYREF
    wchar_t v25[32]; // [esp+20h] [ebp-160h] BYREF
    wchar_t v26[32]; // [esp+60h] [ebp-120h] BYREF
    wchar_t v27[32]; // [esp+A0h] [ebp-E0h] BYREF
    wchar_t a1[32]; // [esp+E0h] [ebp-A0h] BYREF
    char v29[32]; // [esp+120h] [ebp-60h] BYREF
    wchar_t v30[32]; // [esp+140h] [ebp-40h] BYREF

    jkGuiNetHost_LoadSettings(); // Added

    v25[0] = '8';
    v25[1] = 0;
    memset(&v25[2], 0, 0x3Cu);
    v27[0] = 0;
    v26[0] = 0;
    memset(&v27[1], 0, 0x3Cu);
    v27[31] = 0;
    memset(&v26[1], 0, 0x3Cu);
    v26[31] = 0;
    jkGuiNetHost_aElements[6].selectedTextEntry = jkGuiNetHost_gameFlags & MULTIMODEFLAG_SCORELIMIT;
    jkGuiNetHost_aElements[8].selectedTextEntry = jkGuiNetHost_gameFlags & MULTIMODEFLAG_TIMELIMIT;
    jkGuiNetHost_aElements[11].selectedTextEntry = jkGuiNetHost_gameFlags & MULTIMODEFLAG_SINGLE_LEVEL;
    jkGuiNetHost_aElements[10].selectedTextEntry = jkGuiNetHost_gameFlags & MULTIMODEFLAG_TEAMS;
    if ( !jkGuiNetHost_gameName[0] )
    {
        v1 = jkStrings_GetText("GUI_DEFAULT_GAME_NAME");
        jk_snwprintf(jkGuiNetHost_gameName, 0x20u, v1, jkPlayer_playerShortName);
    }
    jkGuiNetHost_aElements[3].wstr = jkGuiNetHost_gameName;
    jkGuiNetHost_aElements[3].selectedTextEntry = 16;
#ifdef QOL_IMPROVEMENTS
    jk_snwprintf(v25, 0x20u, L"%d", jkGuiNetHost_bIsDedicated ? jkGuiNetHost_maxPlayers-1 : jkGuiNetHost_maxPlayers);
#else
    jk_snwprintf(v25, 0x20u, L"%d", jkGuiNetHost_maxPlayers);
#endif
    jkGuiNetHost_aElements[5].wstr = v25;
    jkGuiNetHost_aElements[5].selectedTextEntry = 3;
    jk_snwprintf(v27, 0x20u, L"%d", jkGuiNetHost_scoreLimit);
    //a2[0] = (unsigned int)jkGuiNetHost_timeLimit; wat??
    jkGuiNetHost_aElements[7].wstr = v27;
    jkGuiNetHost_aElements[7].selectedTextEntry = 4;
#ifdef QOL_IMPROVEMENTS
    jk_snwprintf(jkGuiNetHost_portText, 0x20u, L"%d", jkGuiNetHost_portNum);
    jkGuiNetHost_aElements[26].wstr = jkGuiNetHost_portText;
    jkGuiNetHost_aElements[26].selectedTextEntry = 31;
#endif
    jk_snwprintf(v26, 0x20u, L"%d", (unsigned int)(__int64)((double)(unsigned int)jkGuiNetHost_timeLimit * 0.000016666667));
    jkGuiNetHost_aElements[9].wstr = v26;
    jkGuiNetHost_aElements[9].selectedTextEntry = 4;
    __snprintf(v29, 32, "RANK_%d_L", jkGuiNetHost_maxRank); // sprintf -> snprintf
    v21 = jkStrings_GetText(v29);
    v20 = jkGuiNetHost_maxRank;
    v2 = jkStrings_GetText("GUI_RANK");
    jk_snwprintf(jkGuiNetHost_waIdk, 0x80u, v2, v20, v21);
    memset(v30, 0, sizeof(v30));
    jkGuiNetHost_aElements[13].wstr = jkGuiNetHost_waIdk;
    jkGuiNetHost_aElements[17].wstr = v30;
    jkGuiNetHost_aElements[17].selectedTextEntry = 16;
    jkGuiRend_DarrayNewStr(&jkGuiNetHost_dArray1, jkEpisode_var2 + 1, 1);
    jkGuiSingleplayer_sub_41A9B0(&jkGuiNetHost_dArray1, &jkGuiNetHost_aElements[19], 0, 14);
    jkGuiRend_DarrayNewStr(&jkGuiNetHost_dArray2, 10, 1);
    jkGuiRend_DarrayReallocStr(&jkGuiNetHost_dArray2, 0, 0);
    jkGuiRend_SetClickableString(&jkGuiNetHost_aElements[21], &jkGuiNetHost_dArray2);
    jkGuiNetHost_aElements[19].func = jkGuiNetHost_sub_4119D0;
    jkGui_sub_412E20(&jkGuiNetHost_menu, 100, 101, 101);
    jkGuiRend_MenuSetLastElement(&jkGuiNetHost_menu, &jkGuiNetHost_aElements[23]);
    jkGuiRend_SetDisplayingStruct(&jkGuiNetHost_menu, &jkGuiNetHost_aElements[24]);
    jkGuiNetHost_aElements[19].selectedTextEntry = 0;
    jkRes_LoadGob(*((char **)jkGuiNetHost_aElements[19].str + 1));
    if ( jkEpisode_Load(&jkGui_episodeLoad) )
    {
        jkGuiSingleplayer_sub_41AA30(
            &jkGuiNetHost_dArray2,
            &jkGuiNetHost_aElements[21],
            0,
            jkRes_episodeGobName,
            jkGui_episodeLoad.field_0,
            jkGui_episodeLoad.numSeq,
            jkGui_episodeLoad.field_8,
            jkGui_episodeLoad.paEntries);
    }
    else
    {
        jkGuiRend_DarrayFreeEntry(&jkGuiNetHost_dArray2);
        v3 = jkStrings_GetText("GUI_NO_LEVELS_IN_EPISODE");
        jkGuiRend_DarrayReallocStr(&jkGuiNetHost_dArray2, v3, 0);
        jkGuiRend_DarrayReallocStr(&jkGuiNetHost_dArray2, 0, 0);
        jkGuiRend_SetClickableString(&jkGuiNetHost_aElements[21], &jkGuiNetHost_dArray2);
    }
    do
    {
        v4 = jkGuiRend_DisplayAndReturnClicked(&jkGuiNetHost_menu);
        if ( v4 == 1 )
        {
            pMultiEntry->multiModeFlags = 0;
            stdString_SafeWStrCopy(pMultiEntry->serverName, jkGuiNetHost_gameName, 0x20);
            for ( i = __wcschr(pMultiEntry->serverName, ':'); i; i = __wcschr(pMultiEntry->serverName, ':') )
                *i = '-';
            
            stdString_SafeStrCopy(pMultiEntry->episodeGobName, jkGuiNetHost_aElements[19].unistr[jkGuiNetHost_aElements[19].selectedTextEntry].c_str, 0x20);
            stdString_SafeStrCopy(pMultiEntry->mapJklFname, jkGuiNetHost_aElements[21].unistr[jkGuiNetHost_aElements[21].selectedTextEntry].c_str, 0x80);
            
            v10 = wstr_to_int_clamped(v25, 1, 32);

            // Added: Clamping is slightly different for dedicated
            if (jkGuiNetHost_bIsDedicated) {
                v10 += 1;
                if (v10 < 2)
                    v10 = 2;
                if (v10 > 31)
                    v10 = 31;
            }

            pMultiEntry->maxPlayers = v10;
            jkGuiNetHost_maxPlayers = v10;
            pMultiEntry->maxRank = jkGuiNetHost_maxRank;
            v23 = wstr_to_int_clamped(v26, 1, 100);
            v11 = (__int64)((double)v23 * 60000.0);
            pMultiEntry->timeLimit = v11;
            jkGuiNetHost_timeLimit = v11;
            if ( jkGuiNetHost_aElements[8].selectedTextEntry )
            {
                pMultiEntry->multiModeFlags |= MULTIMODEFLAG_TIMELIMIT;
            }
            v13 = wstr_to_int_clamped(v27, 0, 999);
            pMultiEntry->scoreLimit = v13;
            jkGuiNetHost_scoreLimit = v13;
            if ( jkGuiNetHost_aElements[6].selectedTextEntry )
            {
                pMultiEntry->multiModeFlags |= MULTIMODEFLAG_SCORELIMIT;
            }
            if ( jkGuiNetHost_aElements[11].selectedTextEntry )
            {
                pMultiEntry->multiModeFlags |= MULTIMODEFLAG_SINGLE_LEVEL;
            }
            if ( jkGuiNetHost_aElements[10].selectedTextEntry )
                pMultiEntry->multiModeFlags |= (MULTIMODEFLAG_100 | MULTIMODEFLAG_2 | MULTIMODEFLAG_TEAMS);
            stdString_SafeWStrCopy(pMultiEntry->wPassword, v30, 0x20);
            if (_wcslen(pMultiEntry->wPassword)) {
                jkGuiNetHost_sessionFlags |= SESSIONFLAG_PASSWORD; // Added: wtf?
            }
            pMultiEntry->tickRateMs = jkGuiNetHost_tickRate;
            pMultiEntry->sessionFlags = jkGuiNetHost_sessionFlags;
            jkGuiNetHost_gameFlags = pMultiEntry->multiModeFlags;
#ifdef QOL_IMPROVEMENTS
            jkGuiNetHost_portNum = wstr_to_int_clamped(jkGuiNetHost_portText, 1, 65535);
            wuRegistry_SetWString("serverPassword", pMultiEntry->wPassword);
            wuRegistry_SetString("serverEpisodeGob", pMultiEntry->episodeGobName);
            wuRegistry_SetString("serverMapJkl", pMultiEntry->mapJklFname);
            if (jkGuiNetHost_bIsDedicated) {
                jkGuiNetHost_sessionFlags |= SESSIONFLAG_ISDEDICATED;
            }

            if (jkGuiNetHost_bIsCoop) {
                jkGuiNetHost_gameFlags |= MULTIMODEFLAG_COOP;
            }
            pMultiEntry->multiModeFlags = jkGuiNetHost_gameFlags;
            pMultiEntry->sessionFlags = jkGuiNetHost_sessionFlags;
#endif
        }
        else if ( v4 == 200 )
        {
            a1[0] = 0;
            memset(&a1[1], 0, 0x3Cu);
            a1[31] = 0;
            jk_snwprintf(a1, 0x20u, L"%d", jkGuiNetHost_tickRate);
            jkGuiNetHost_aSettingsElements[3].wstr = a1;
            jkGuiNetHost_aSettingsElements[3].selectedTextEntry = 32;
#ifdef QOL_IMPROVEMENTS
            jkGuiNetHost_aSettingsElements[4].selectedTextEntry = jkGuiNetHost_bIsDedicated;
            jkGuiNetHost_aSettingsElements[5].selectedTextEntry = jkGuiNetHost_bIsCoop;
#endif
            jkGuiRend_MenuSetLastElement(&jkGuiNetHost_menuSettings, &jkGuiNetHost_aSettingsElements[4]);
            jkGuiRend_SetDisplayingStruct(&jkGuiNetHost_menuSettings, &jkGuiNetHost_aSettingsElements[5]);
            if ( jkGuiRend_DisplayAndReturnClicked(&jkGuiNetHost_menuSettings) == 1 )
            {
                pMultiEntry->sessionFlags = 0;
                pMultiEntry->tickRateMs = wstr_to_int_clamped(a1, TICKRATE_MIN, TICKRATE_MAX);
                jkGuiNetHost_tickRate = pMultiEntry->tickRateMs;
                jkGuiNetHost_sessionFlags = pMultiEntry->sessionFlags;
            }
#ifdef QOL_IMPROVEMENTS
            jkGuiNetHost_bIsDedicated = !!jkGuiNetHost_aSettingsElements[4].selectedTextEntry;
            jkGuiNetHost_bIsCoop = !!jkGuiNetHost_aSettingsElements[5].selectedTextEntry;
#endif
        }
    }
    while ( v4 == 200 );
    jkGuiNetHost_SaveSettings(); // Added: Make sure stuff actually gets into the registry
    jkGuiRend_DarrayFree(&jkGuiNetHost_dArray1);
    jkGuiRend_DarrayFree(&jkGuiNetHost_dArray2);
    return v4;
}

int jkGuiNetHost_sub_4118C0(jkMultiEntry3 *pEntry)
{
    int v1; // edi
    int tickRate; // eax
    wchar_t a1a[32]; // [esp+8h] [ebp-40h] BYREF

    a1a[0] = 0;
    memset(&a1a[1], 0, 0x3Cu);
    a1a[31] = 0;
    jk_snwprintf(a1a, 0x20u, L"%d", jkGuiNetHost_tickRate);
    jkGuiNetHost_aSettingsElements[3].wstr = a1a;
    jkGuiNetHost_aSettingsElements[3].selectedTextEntry = 32;
    jkGuiRend_MenuSetLastElement(&jkGuiNetHost_menuSettings, &jkGuiNetHost_aSettingsElements[4]);
    jkGuiRend_SetDisplayingStruct(&jkGuiNetHost_menuSettings, &jkGuiNetHost_aSettingsElements[5]);
    v1 = jkGuiRend_DisplayAndReturnClicked(&jkGuiNetHost_menuSettings);
    if ( v1 == 1 )
    {
        pEntry->sessionFlags = 0;
        tickRate = wstr_to_int_clamped(a1a, TICKRATE_MIN, TICKRATE_MAX);
        pEntry->tickRateMs = tickRate;
        jkGuiNetHost_tickRate = tickRate;
        jkGuiNetHost_sessionFlags = pEntry->sessionFlags;
    }
    return v1;
}

int jkGuiNetHost_sub_4119D0(jkGuiElement *pElement, jkGuiMenu *pMenu, int mouseX, int mouseY, int redraw)
{
    wchar_t *v5; // eax

    if ( mouseX != -1 || mouseY != -1 )
        jkGuiRend_ClickSound(pElement, pMenu, mouseX, mouseY, redraw);
    jkRes_LoadGob(pElement->unistr[pElement->selectedTextEntry].c_str);
    if ( jkEpisode_Load(&jkGui_episodeLoad) )
    {
        jkGuiSingleplayer_sub_41AA30(
            &jkGuiNetHost_dArray2,
            &jkGuiNetHost_aElements[21],
            0,
            jkRes_episodeGobName,
            jkGui_episodeLoad.field_0,
            jkGui_episodeLoad.numSeq,
            jkGui_episodeLoad.field_8,
            jkGui_episodeLoad.paEntries);
    }
    else
    {
        jkGuiRend_DarrayFreeEntry(&jkGuiNetHost_dArray2);
        v5 = jkStrings_GetText("GUI_NO_LEVELS_IN_EPISODE");
        jkGuiRend_DarrayReallocStr(&jkGuiNetHost_dArray2, v5, 0);
        jkGuiRend_DarrayReallocStr(&jkGuiNetHost_dArray2, 0, 0);
        jkGuiRend_SetClickableString(&jkGuiNetHost_aElements[21], &jkGuiNetHost_dArray2);
    }
    if ( mouseX != -1 || mouseY != -1 )
        jkGuiRend_UpdateAndDrawClickable(&jkGuiNetHost_aElements[21], &jkGuiNetHost_menu, 1);
    return 0;
}

int jkGuiNetHost_sub_411AE0(jkGuiElement *pElement, jkGuiMenu *pMenu, int mouseX, int mouseY, int redraw)
{
    wchar_t *v7; // eax
    int v9; // [esp-8h] [ebp-28h]
    wchar_t *v11; // [esp-4h] [ebp-24h]
    char v12[32]; // [esp+0h] [ebp-20h] BYREF

    if ( pElement->hoverId == 1 )
    {
        if ( jkGuiNetHost_maxRank )
        {
            stdString_snprintf(v12, 32, "RANK_%d_L", --jkGuiNetHost_maxRank);
            v11 = jkStrings_GetText(v12);
            v9 = jkGuiNetHost_maxRank;
            v7 = jkStrings_GetText("GUI_RANK");
            jk_snwprintf(jkGuiNetHost_waIdk, 0x80u, v7, v9, v11);
            jkGuiNetHost_aElements[13].wstr = jkGuiNetHost_waIdk;
            jkGuiRend_UpdateAndDrawClickable(&jkGuiNetHost_aElements[13], pMenu, 1);
        }
    }
    else if ( pElement->hoverId == 2 && (unsigned int)jkGuiNetHost_maxRank < 8 )
    {
        stdString_snprintf(v12, 32, "RANK_%d_L", ++jkGuiNetHost_maxRank);
        jk_snwprintf(jkGuiNetHost_waIdk, 0x80u, jkStrings_GetText("GUI_RANK"), jkGuiNetHost_maxRank, jkStrings_GetText(v12));
        jkGuiNetHost_aElements[13].wstr = jkGuiNetHost_waIdk;
        jkGuiRend_UpdateAndDrawClickable(&jkGuiNetHost_aElements[13], pMenu, 1);
        return 0;
    }
    return 0;
}