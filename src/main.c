#include <genesis.h>
#include "resources.h"

#define FLOOR_Y 184

static s16 playerX = 40;
static s16 playerLife = 5;
static s16 drivEnergy = 0;
static s16 enemyX = 184;
static s16 enemyLife = 3;
static s16 bossX = 264;
static s16 bossLife = 8;
static u16 frameCount = 0;
static char status[41];
static Sprite *rynSprite;
static Sprite *raiderSprite;
static Sprite *vorathSprite;

static u16 closeEnough(s16 a, s16 b, s16 distance)
{
    s16 delta = a - b;
    if (delta < 0) delta = -delta;
    return delta < distance;
}

static void drawScene(void)
{
    s16 i;
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    VDP_setTextPalette(PAL0);
    VDP_drawText("DRIVKANO - VILA DE BRIM", 7, 1);
    sprintf(status, "VIDA %d   DRIV %d   PEDRAS 0/4", playerLife, drivEnergy);
    VDP_drawText(status, 2, 3);
    VDP_drawText("TEMPestade eterna: busque as Pedras de Driv", 1, 5);
    VDP_drawText("       *   *       *       *", 1, 11);
    VDP_drawText("     ^^^       ^^^       ^^^", 1, 12);
    VDP_drawText("   ^^^^^^^   ^^^^^^^   ^^^^^^^", 1, 13);
    VDP_drawText("[BRIM]       [RUINAS]       [TEMPESTADE]", 1, 17);
    for (i = 0; i < 40; i++) status[i] = (i & 1) ? '_' : '=';
    status[40] = 0;
    VDP_drawText(status, 0, 23);
    if (playerLife <= 0) VDP_drawText("RYN CAIU! START PARA TENTAR DE NOVO", 3, 21);
    else if (bossLife <= 0) VDP_drawText("A TEMPESTADE CEDEU! DEMO CONCLUIDA", 3, 21);
    else if (enemyLife <= 0) VDP_drawText("INIMIGO DERROTADO! SIGA A DIREITA", 3, 21);
    VDP_drawText("D-PAD MOVE | A ATACA | B RAIO | START REINICIA", 0, 27);
}

int main(void)
{
    u16 joy;
    JOY_init();
    VDP_setScreenWidth320();
    SPR_init();
    rynSprite = SPR_addSprite(&Ryn, playerX, FLOOR_Y - 32, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    raiderSprite = SPR_addSprite(&Raider, enemyX, FLOOR_Y - 32, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    vorathSprite = SPR_addSprite(&Vorath, bossX, FLOOR_Y - 64, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    while (TRUE)
    {
        joy = JOY_readJoypad(JOY_1);
        frameCount++;
        if (joy & BUTTON_START) {
            playerLife = 5;
            drivEnergy = 0;
            enemyLife = 3;
            bossLife = 8;
            playerX = 40;
            enemyX = 184;
            bossX = 264;
        }
        if ((joy & BUTTON_LEFT) && playerX > 4) playerX -= 2;
        if ((joy & BUTTON_RIGHT) && playerX < 280) playerX += 2;
        if (joy & BUTTON_A) {
            if (enemyLife > 0 && closeEnough(playerX, enemyX, 40)) {
                enemyLife--;
                if (drivEnergy < 9) drivEnergy++;
            } else if (bossLife > 0 && closeEnough(playerX, bossX, 48)) {
                bossLife--;
                if (drivEnergy < 9) drivEnergy++;
            }
        }
        if ((joy & BUTTON_B) && drivEnergy > 0) {
            if (bossLife > 0) bossLife = (bossLife > 2) ? bossLife - 2 : 0;
            else if (enemyLife > 0) enemyLife = 0;
            drivEnergy--;
        }
        if ((frameCount % 90) == 0 && playerLife > 0 && enemyLife > 0) playerLife--;
        if (enemyLife > 0 && (frameCount % 4) == 0) {
            if (enemyX > playerX + 22) enemyX--;
            else if (enemyX < playerX - 22) enemyX++;
        }
        SPR_setPosition(rynSprite, playerX, FLOOR_Y - 32);
        SPR_setPosition(raiderSprite, enemyX, FLOOR_Y - 32);
        SPR_setVisibility(raiderSprite, enemyLife > 0 ? VISIBILITY_ALWAYS : VISIBILITY_HIDE);
        SPR_setPosition(vorathSprite, bossX, FLOOR_Y - 64);
        SPR_setVisibility(vorathSprite, bossLife > 0 ? VISIBILITY_ALWAYS : VISIBILITY_HIDE);
        drawScene();
        SPR_update();
        SYS_doVBlankProcess();
    }
    return 0;
}
