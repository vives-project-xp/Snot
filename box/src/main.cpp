#include "mbed.h"
#include "Board.h"
#include "StringReader.h"
#include "GameHandler.h"
#include "mbed_trace.h"
#include "mbed_events.h"
#include <utility>
// #include <SDFileSystem.h>
 
// SDFileSystem sd(p5, p6, p7, p8, "sd"); // the pinout on the mbed Cool Components workshop board

#define TRACE_GROUP "MAIN"

BufferedSerial pc(USBTX, USBRX, 115200);
EventQueue queue;

Board* board;
GameHandler* gameHandler;

char buffer[2048];

void setup_game_handler(void) {
  //ConfigReader * reader = new StringReader("{\"name\":\"LocationGame\",\"box_description\":\"Go to locations.\",\"full_description\":\"Whilst bringing the rebels to the imperial prison, your stardestroyer got shot. Retrieve the lost communicator parts to contact an extraction team\",\"length\":3000,\"time\":120,\"missions\":[{\"id\":1,\"name\":\"GotoLocation\",\"description\":\"The first part is at the entrance of Kinepolis\",\"typeid\":10,\"locations\":[{\"lat\":51.223142,\"long\":2.896536,\"radius\":12.5}]},{\"id\":2,\"name\":\"GotoLocation\",\"description\":\"The next one is at the Spar\",\"typeid\":10,\"locations\":[{\"lat\":51.222359,\"long\":2.892109,\"radius\":12.5}]},{\"id\":3,\"name\":\"FindAKey\",\"description\":\"There is a key on the light pole\",\"typeid\":20}]}");
  ConfigReader * reader = new StringReader("{\"name\":\"LocationGame\",\"box_description\":\"Go to locations.\",\"full_description\":\"Whilst bringing the rebels to the imperial prison, your stardestroyer got shot. Retrieve the lost communicator parts to contact an extraction team\",\"length\":3000,\"time\":120,\"missions\":[{\"id\":1,\"name\":\"FindAKey\",\"description\":\"There is a key on orange bottle\",\"typeid\":20},{\"id\":2,\"name\":\"FindAKey\",\"description\":\"There is a key in stiftrand van het bord\",\"typeid\":20}]}]}");

  reader->read(buffer, sizeof(buffer));
  printf("\n Content: --------------------------- \n %s \n ------------------------------------------- \n", buffer);

  gameHandler = new GameHandler(buffer, board, &queue);
}

void init()
{
    mbed_trace_init();
    tr_info("Initializing Vives CityGame");

    board = new Board(&queue);

    setup_game_handler();
}

int main(void)
{
    init();
    // ThisThread::sleep_for(1s);
    // printf("Hello World!\n");   
 
    // mkdir("/sd/mydir", 0777);
    
    // FILE *fp = fopen("/sd/mydir/sdtest.txt", "w");
    // if(fp == NULL) {
    //     error("Could not open file for write\n");
    // }
    // fprintf(fp, "Hello fun SD Card World!");
    // fclose(fp); 
 
    // printf("Goodbye World!\n");
    while(true){
        gameHandler->run();
        queue.dispatch_once();
    }
    return 0;
}
