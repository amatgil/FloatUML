#include "defs.h"
#include "umla.h"
#include "umlr.h"
#include "utils.h"
#include "intersect.h"
#include "raymath.h"

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

        if (pt1.x == arect.x || pt1.x == (arect.x + arect.width))
        {
            if (pt1.x == arect.x) 
            {
                if (Vector2Angle((Vector2){0,1}, (Vector2){punt_mig.x-pt1.x, punt_mig.y-pt1.y}) > PI/2) pt1.y += 10;
                else pt1.y -= 20;
                pt1.x -= strlen(buff)*15;
            }
            else 
            {
                if (Vector2Angle((Vector2){0,-1}, (Vector2){punt_mig.x-pt1.x, punt_mig.y-pt1.y}) > PI/2) pt1.y += 10;
                else pt1.y -= 20;
                pt1.x += 10;
            }
        }
        else if (pt1.y == arect.y || pt1.y == (arect.y + arect.height))
        {
            if (pt1.y == arect.y) 
            {
                if (Vector2Angle((Vector2){1,0}, (Vector2){punt_mig.x-pt1.x, punt_mig.y-pt1.y}) > PI/2) pt1.x += 30;
                else pt1.x -= 30;
                pt1.y -= 20;
            }
            else    
            {
                if (Vector2Angle((Vector2){-1,0}, (Vector2){punt_mig.x-pt1.x, punt_mig.y-pt1.y}) > PI/2) pt1.x += 30;
                else pt1.x -= 30;
                pt1.y += 10;
            }
        }
        DrawTextEx(style->font, buff, pt1, 20, 0, BLACK);
    }

    if (lenght > 2)
    {
        Rectangle rec_l = (Rectangle) {punt_mig.x, punt_mig.y, 40, 40};
        DrawRectanglePro(rec_l, (Vector2) {20, 20}, 45, BLACK);
        Rectangle rec_c = (Rectangle) {punt_mig.x, punt_mig.y, 36, 36};
        DrawRectanglePro(rec_c, (Vector2) {18, 18}, 45, WHITE);
    }


    if (relacio.associativa != NULL) {
        umld_assoc(relacio.associativa, punt_mig, style);
    }
}

void draw_y (Vector2 center_father, Vector2 center_son, Rectangle arect, Rectangle brect)
{
    DrawLineEx(center_father, (Vector2) {center_father.x, (center_father.y+center_son.y)/2}, 3, BLACK);
    DrawLineEx((Vector2) {center_father.x, (center_father.y+center_son.y)/2}, (Vector2) {center_son.x, (center_father.y+center_son.y)/2}, 3, BLACK);
    DrawLineEx(center_son, (Vector2) {center_son.x, (center_father.y+center_son.y)/2}, 3, BLACK);
}

void draw_x (Vector2 center_father, Vector2 center_son, Rectangle arect, Rectangle brect)
{
    DrawLineEx(center_father, (Vector2) {(center_father.x+center_son.x)/2, center_father.y}, 3, BLACK);
    DrawLineEx((Vector2) {(center_father.x+center_son.x)/2, center_father.y}, (Vector2) {(center_father.x+center_son.x)/2, center_son.y}, 3, BLACK);
    DrawLineEx(center_son, (Vector2) {(center_father.x+center_son.x)/2, center_son.y}, 3, BLACK);
}

void draw_subclass_relation(struct Classe father, struct Classe son, struct Style *style)
{
    Rectangle arect = umld_rect_of(father, style);
    Rectangle brect = umld_rect_of(son, style);
    Vector2 center_father = rect_center(arect);
    Vector2 center_son = rect_center(brect);
    Vector2 pt1 =
              int_seg_rect(center_father, center_son, arect);
    Vector2 pt2 =
              int_seg_rect(center_father, center_son, brect);
    if (pt1.x == pt2.x || pt1.y == pt2.y) DrawLineEx(pt1, pt2, 3, BLACK);
    else if (pt1.y < pt2.y) draw_y (center_father, center_son, arect, brect);
    else if (abs(pt1.x-pt2.x) > pt1.y-pt2.y) draw_x (center_father, center_son, arect, brect);
    else draw_y (center_father, center_son, arect, brect);
}