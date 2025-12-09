#include <iostream>
//#include <cstdlib>
//#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <stdlib.h>
#include<time.h>   

// Inkludera CURL-huvudfilen
#include <curl/curl.h>





int main() {
    // Sätt startvärdet för slumptalsgeneratorn
    srand(std::time(nullptr));

    // Initialisera CURL globalt (bra praxis)
    curl_global_init(CURL_GLOBAL_ALL);

    std::cout << "Simulering av IoT-enhet startad." << std::endl;
    std::cout << "Skickar data var 30:e sekund..." << std::endl;

    while (true) {
        // 1. Läs sensorvärdet (slumpmässigt tal)
        int sensor_data = read_sensor();

        // 2. Skicka värdet
        send_data(sensor_data);
        
        // 3. Vänta 30 sekunder (ett halvt minut)
        std::cout << "--- Pausar i 30 sekunder... ---" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }

    // Rensa upp CURL globalt (tekniskt sett nås aldrig p.g.a. while(true))
    curl_global_cleanup();
    
    return 0;
}