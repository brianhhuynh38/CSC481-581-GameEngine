#pragma once
#ifndef NETWORK_H
#define NETWORK_H

#define MAX_PACKET 0xFF
#define MAX_SOCKETS 0x10

#define WOOD_WAIT_TIME 5000

#define FLAG_QUIT 0x0000
#define FLAG_WOOD_UPDATE 0x0010

#define within(num) (int) ((float) num * rand() / (RAND_MAX + 1.0))

#endif