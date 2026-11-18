#include <genesis.h>

#define FLOOR_Y 24

static s16 playerX = 5;
static s16 playerLife = 5;
static s16 drivEnergy = 0;
static s16 enemyX = 23;
static s16 enemyLife = 3;
static s16 bossX = 33;
static s16 bossLife = 8;
static u16 frameCount = 0;
static char status[41];
static char floorLine[41];

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
    for (i = 0; i < 40; i++) floorLine[i] = (i & 1) ? '_' : '=';
    floorLine[40] = 0;
    VDP_drawText(floorLine, 0, FLOOR_Y);
    VDP_drawText("RYN", playerX, FLOOR_Y - 2);
    VDP_drawText("@", playerX + 1, FLOOR_Y - 1);
    if (enemyLife > 0) {
        VDP_drawText("G", enemyX, FLOOR_Y - 1);
        VDP_drawText("INIMIGO", enemyX > 30 ? 28 : enemyX - 2, FLOOR_Y - 3);
    }
    if (bossLife > 0) {
        VDP_drawText("V", bossX, FLOOR_Y - 1);
        VDP_drawText("VORATH", bossX - 2, FLOOR_Y - 3);
    }
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
    while (TRUE)
    {
        joy = JOY_readJoypad(JOY_1);
        frameCount++;
        if (joy & BUTTON_START) {
            playerLife = 5;
            drivEnergy = 0;
            enemyLife = 3;
            bossLife = 8;
            playerX = 5;
            enemyX = 23;
            bossX = 33;
        }
        if ((joy & BUTTON_LEFT) && playerX > 1) playerX--;
        if ((joy & BUTTON_RIGHT) && playerX < 36) playerX++;
        if (joy & BUTTON_A) {
            if (enemyLife > 0 && (playerX - enemyX < 4) && (enemyX - playerX < 4)) {
                enemyLife--;
                if (drivEnergy < 9) drivEnergy++;
            } else if (bossLife > 0 && (playerX - bossX < 5) && (bossX - playerX < 5)) {
                bossLife--;
                if (drivEnergy < 9) drivEnergy++;
            }
        }
        if ((joy & BUTTON_B) && drivEnergy > 0) {
            if (bossLife > 0) bossLife = (bossLife > 2) ? bossLife - 2 : 0;
            else if (enemyLife > 0) enemyLife = 0;
            drivEnergy--;
        }
        if ((frameCount % 45) == 0 && playerLife > 0 && enemyLife > 0) playerLife--;
        if (enemyLife > 0 && (frameCount % 20) == 0) {
            if (enemyX > playerX + 2) enemyX--;
            else if (enemyX < playerX - 2) enemyX++;
        }
        drawScene();
        SYS_doVBlankProcess();
    }
    return 0;
}
