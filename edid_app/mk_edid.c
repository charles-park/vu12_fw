/*---------------------------------------------------------------------------*/
/**
 * @file mk_edid.c
 * @author charles-park (charles.park@hardkernel.com)
 * @brief create lt8619c fixed edid for control timing
 * @version 0.1
 * @date 2024-02-22
 *
 * gcc -o edid mk_edid.c
 *
 * @copyright Copyright (c) 2022
**/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*---------------------------------------------------------------------------*/
uint8_t ONCHIP_EDID[256] = {
    //1920*1080
    0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x0e, 0xd4, 0x32, 0x31, 0x00, 0x88, 0x88, 0x88,
    0x20, 0x1c, 0x01, 0x03, 0x80, 0x0c, 0x07, 0x78, 0x0a, 0x0d, 0xc9, 0xa0, 0x57, 0x47, 0x98, 0x27,
    0x12, 0x48, 0x4c, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3a, 0x80, 0x18, 0x71, 0x38, 0x2d, 0x40, 0x58, 0x2c,
    0x45, 0x00, 0x80, 0x38, 0x74, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x0a,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x4C, 0x6F, 0x6E, 0x74, 0x69, 0x75, 0x6D, 0x20, 0x73, 0x65, 0x6D, 0x69, 0x20, 0x01, 0xf5,

    0x02, 0x03, 0x12, 0xf1, 0x23, 0x09, 0x07, 0x07, 0x83, 0x01, 0x00, 0x00, 0x65, 0x03, 0x0c, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf
};

/*---------------------------------------------------------------------------*/
typedef struct {
    uint32_t pix_clk;
    uint16_t hfp;
    uint16_t hs;
    uint16_t hbp;
    uint16_t hact;
    uint16_t htotal;
    uint16_t vfp;
    uint16_t vs;
    uint16_t vbp;
    uint16_t vact;
    uint16_t vtotal;
}   video_timing;

//  52800, 50, 50, 40, 1920, 1100, 50, 20, 10, 720, 800
const video_timing  lcd_timing = {
    52800,  // pixel clock(Khz)

    // lcd horizontal data
    50,     // hfp
    50,     // hs
    40,     // hbp
    960,    // hact = (real view area, 960 * 2(LVDS ch) = 1920)
    1100,   // htotal = hact + hbp + hs + hfp

    // lcd vertical data
    50,     // vfp
    20,     // vs
    10,     // vbp
    720,    // vact = (real view area, 720)
    800     // vtotal = vact + vbp + vs + vfp
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void        lt8619c_edid_dtbcal     (uint8_t *pbuf, const video_timing *pt)
{
    uint16_t h_blk, v_blk, pix_clk;
    pix_clk = (pt->pix_clk / 10);
    h_blk = pt->hfp + pt->hs + pt->hbp;  // h blanking total
    v_blk = pt->vfp + pt->vs + pt->vbp;  // v blanking total

    pbuf [0]  = (pix_clk % 256);    // pix_clk low
    pbuf [1]  = (pix_clk / 256);    // pix_clk high

    pbuf [2]  = (pt->hact % 256);   // hact low
    pbuf [3]  = (h_blk % 256);      // h blanking low
    pbuf [4]  = ((pt->hact / 256)<<4) + (h_blk / 256);  // hact : h blanking high

    pbuf [5]  = (pt->vact % 256);   // vact low
    pbuf [6]  = (v_blk % 256);      // v blanking low
    pbuf [7]  = ((pt->vact / 256)<<4) + (v_blk / 256);  // vact : v blanking high

    pbuf [8]  = (pt->hfp % 256);    // H sync offset low
    pbuf [9]  = (pt->hs  % 256);    // H sync width low

    pbuf [10] = ((pt->vfp % 256) <<4) + (pt->vs % 256);
    pbuf [11] = ((pt->hfp / 256) <<6) + ((pt->hs / 256) <<4) +
                ((pt->vfp / 256) <<2) + (pt->vs / 256);

    pbuf [17] = 0x1E;   // progress V+ H+
}

/*---------------------------------------------------------------------------*/
uint8_t     lt8619c_edid_checksum   (uint8_t block, uint8_t *pbuf)
{
    uint8_t i, cksum, *p = (pbuf + 128 * block);

    for (i = 0, cksum = 0; i < 127; i++) {
        cksum += p [i];     cksum %= 256;
    }
    cksum = 256 - cksum;
    return cksum;
}

/*---------------------------------------------------------------------------*/
int main ()
{
    int i;

    lt8619c_edid_dtbcal (ONCHIP_EDID + 0x36, &lcd_timing);
    ONCHIP_EDID[127] = lt8619c_edid_checksum (0, &ONCHIP_EDID[0]);  //edid_check_sum;

    printf ("\r\nCalculate EDID data [0 ~ 127]\r\n");
    printf ("\r\n");
    printf ("pix_clk = %d \r\n", lcd_timing.pix_clk);
    printf ("\r\n");
    printf ("hfp    = %d \r\n", lcd_timing.hfp);
    printf ("hs     = %d \r\n", lcd_timing.hs);
    printf ("hbp    = %d \r\n", lcd_timing.hbp);
    printf ("hact   = %d \r\n", lcd_timing.hact);
    printf ("htotal = %d \r\n", lcd_timing.htotal);
    printf ("\r\n");
    printf ("vfp    = %d \r\n", lcd_timing.vfp);
    printf ("vs     = %d \r\n", lcd_timing.vs);
    printf ("vbp    = %d \r\n", lcd_timing.vbp);
    printf ("vact   = %d \r\n", lcd_timing.vact);
    printf ("vtotal = %d \r\n", lcd_timing.vtotal);
    printf ("\r\n");

    printf ("*** pix_clk cal = %d \r\n", (lcd_timing.htotal * lcd_timing.vtotal * 60)/1000);
    printf ("\r\n");
    for (i = 0; i < 128; i++) {
        if (i && ((i % 16) == 0))   printf ("\r\n");
        printf ("0x%02x, ", ONCHIP_EDID[i]);
    }
    printf ("\r\n");
    printf ("\r\n");
    printf ("EDID[127](chksum 0 ~ 126) = 0x%02x\r\n", lt8619c_edid_checksum (0, &ONCHIP_EDID[0]));  //edid_check_sum;
    printf ("\r\n");
    return 0;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/








