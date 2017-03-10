#pragma once

inline unsigned char DectoBCD(unsigned char temp )
{
    return ( (temp/16)*10 + temp%16 ) ;
}