#include "defs.h"
#include "umla.h"
#include "umlr.h"
#include "utils.h"
#include "intersect.h"

void draw_relation(struct Relacio relacio, struct Style *style) {
    Vector2 punt_mig = calcul_punt_mig (relacio, style);
    int lenght = relacio.len;
    for (int i = 0; i < lenght; ++i)
    {
        Rectangle arect = umld_rect_of(*relacio.cs[i], style);
        Vector2 pt1 =
                  int_seg_rect(rect_center(arect), punt_mig, arect);
        DrawLineEx(pt1,punt_mig,3,BLACK);
        char buff[2048];
        if (relacio.multiplicitats[i].y == (float)-1.) sprintf(buff,"%.0f..*", relacio.multiplicitats[i].x);
        else sprintf(buff,"%.0f..%.0f", relacio.multiplicitats[i].x, relacio.multiplicitats[i].y);
        if (punt_mig.x-pt1.x < 0) pt1.x -= strlen(buff)*7;
        else pt1.x += 2;
        if (punt_mig.y-pt1.y < 0) pt1.y -= strlen(buff)*2.5;
        else pt1.y += 2; 
        DrawTextEx(style->font, buff, pt1, 12, 0, BLACK);
    }
}
