#ifndef ROOMSCRIPTDEFINITIONS_H_
#define ROOMSCRIPTDEFINITIONS_H_

#define L(x) ((x)<<3)
#define T(x) ((x)<<4)
#define TT(x,y) T(x),T(y)
#define ENEMY_RANDOM_LOCATION -1234567
#define EERAND(x,y) ENEMY_RANDOM_LOCATION,ENEMY_RANDOM_LOCATION
#define MAXEDGE 999999
#define PRESERVE -1
#define RIGHTEDGE {MAXEDGE,-10}, {MAXEDGE,MAXEDGE + 10}, {-9,PRESERVE}
#define LEFTEDGE {-20,-10}, {-10,MAXEDGE + 10}, {MAXEDGE,PRESERVE}
#define BOTTOMEDGE {-10,MAXEDGE}, {MAXEDGE + 10,MAXEDGE}, {PRESERVE, -9}
#define BOTTOMEDGE_L {0,MAXEDGE}, {L(30),MAXEDGE}, {PRESERVE, -9}
#define BOTTOMEDGE_R {L(30),MAXEDGE}, {MAXEDGE,MAXEDGE}, {L(14), -9}
#define TOPEDGE {-10,-20}, {MAXEDGE + 10, -10}, {PRESERVE,MAXEDGE}
#define TOPEDGE_L {0,-20}, {L(30),-10}, {PRESERVE, MAXEDGE}
#define TOPEDGE_R {L(30),-20}, {MAXEDGE,-10}, {L(14), MAXEDGE}

#define EDGE_BOTTOM_TO_TOP(a,b) {L(30*(a)),MAXEDGE}, {L(30*((a)+1)),MAXEDGE}, {L(14) + L(30*(b)), -9}
#define EDGE_TOP_TO_BOTTOM(a,b) {L(30*(a)),-20}, {L(30*((a)+1)),-10}, {L(14) + L(30*(b)), MAXEDGE}
#define EDGE_RIGHT_TO_LEFT(a,b) {MAXEDGE,L(20*(a))}, {MAXEDGE,L(20*((a)+1))}, {-9, L(8) + L(20*(b))}
#define EDGE_LEFT_TO_RIGHT(a,b) {-20,L(20*(a))}, {-10,L(20*((a)+1))}, {MAXEDGE, L(8) + L(20*(b))}

#define CAVE_TO_EDGE_BOTTOM(a,b) {T(a-1),T(b)},{T(a+1),T(b+1)},{L(14),MAXEDGE}
#define CAVE_TO_EDGE_BOTTOM_X(a,b,c) {T(a-1),T(b)},{T(a+1),T(b+1)},{T(c),MAXEDGE}
#define CAVE_TO_CAVE(a,b,c,d) {T(a-1),T(b)},{T(a+1),T(b+1)},{T(c),T(d)}
#define BOTTOM_TO_CAVE(a,b) {0,MAXEDGE},{MAXEDGE,MAXEDGE}, {T(a),T(b + 1) + 1}
#define BOTTOM_L_TO_CAVE(a,b) {0,MAXEDGE}, {L(30),MAXEDGE}, {T(a),T(b + 1)  + 1}
#define BOTTOM_R_TO_CAVE(a,b) {L(30),MAXEDGE}, {MAXEDGE,MAXEDGE}, {T(a),T(b + 1)  + 1}
#define BOTTOM_LL_TO_CAVE(a,b) {0,MAXEDGE}, {L(15),MAXEDGE}, {T(a),T(b + 1)  + 1}
#define BOTTOM_LR_TO_CAVE(a,b) {L(15),MAXEDGE}, {MAXEDGE,MAXEDGE}, {T(a),T(b + 1)  + 1}

#define STAIRS_TO(a,b,x,y) {a*16,b*16},{a*16+32,b*16+32},{x*16,y*16},true

#define BOTTOMEDGE_TO(x) {0,MAXEDGE}, {MAXEDGE,MAXEDGE}, {(x), -9}
#define LEFTEDGE_TO(x) {-20,0}, {-10,MAXEDGE}, {MAXEDGE,(x)}
#define TOPEDGE_TO(x) {0,-20}, {MAXEDGE, -10}, {(x),MAXEDGE}
#define RIGHTEDGE_TO(x) {MAXEDGE,0}, {MAXEDGE,MAXEDGE}, {-9,(x)}

#endif /*ROOMSCRIPTDEFINITIONS_H_*/
