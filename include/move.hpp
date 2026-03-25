#pragma once
#include "types.hpp"
struct Move{
    uint16_t raw;
    // [4 bits -> flags] [6 bits -> to] [6 bits -> from]
    // 0x3f is the 6 1s mask  // 0xf is the 4 1s mask
    
    /* FLAGS
    0 	0 	0 	0 	quiet moves
  	0 	0 	0 	1 	double pawn push
  	0 	0 	1 	0 	king castle
  	0 	0 	1 	1 	queen castle
 	0 	1 	0 	0 	captures
 	0 	1 	0 	1 	ep-capture
 	1 	0 	0 	0 	knight-promotion
 	1 	0 	0 	1 	bishop-promotion
 	1 	0 	1 	0 	rook-promotion
 	1 	0 	1 	1 	queen-promotion
 	1 	1 	0 	0 	knight-promo capture
 	1 	1 	0 	1 	bishop-promo capture
 	1 	1 	1 	0 	rook-promo capture
 	1 	1 	1 	1 	queen-promo capture 
    */
    
    uint8_t from (){return raw & 0x3f;}
    uint8_t to   (){return (raw>>6) & 0x3f;}
    uint8_t flags(){return (raw>>12) & 0xf;}

    bool isCapture   (){return ((raw>>12) & 0x4) != 0;}
    void setCapture  (){raw |= (0x4 << 12);}
    void resetCapture(){raw &= ~(0x4 << 12);}
    
    void setFrom(int from){
        raw &= 0xffc0; //clears first 4 bits
        raw |= (from & 0x3f); //copies the first 6 bits into place
    }
    void setTo(int to){
        raw &= 0xf03f;
        raw |= ((to & 0x3f)<<6);
    }

    Move(){};
    Move(int from, int to){
        setFrom(from);
        setTo(to);
    }
};