
// pacman -S mingw-w64-x86_64-curl   (GÄLLER ENBART FÖR WINDOWS/MSYS2)

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>

// Inkludera CURL-huvudfilen
#include <curl/curl.h>

// Den API-nyckel och bas-URL du angav:
const std::string API_KEY = "W5YCCCYFWJM9UD34"; 
const std::string BASE_URL = "https://api.thingspeak.com/update?api_key=";

// Callback-funktion som används av libcurl (nödvändig även om svaret ignoreras)
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    (void)contents; // Markera som oanvänd
    (void)userp;    // Markera som oanvänd
    return size * nmemb;
}

// Funktion som simulerar en sensor
int read_sensor() {
    // Genererar ett slumpmässigt tal mellan 1 och 100
    return (std::rand() % 100) + 1;
}

// Funktion som skickar data till Thingspeak
void send_data(int sensor_value) {
    CURL *curl;
    CURLcode res;

    // Bygg hela URL:en
    std::string full_url = BASE_URL + API_KEY + "&field1=" + std::to_string(sensor_value);

    // Initialisera CURL-sessionen
    curl = curl_easy_init();
    if(curl) {
        std::cout << "-> Skickar data: " << sensor_value << std::endl;

        // Ange URL:en
        curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());

        // Callback för att ta emot/ignorera data från servern
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Utför begäran
        res = curl_easy_perform(curl);

        // Kontrollera om det uppstod ett fel
        if(res != CURLE_OK) {
            std::cerr << "!!! curl_easy_perform() misslyckades: " 
                      << curl_easy_strerror(res) << std::endl;
        } else {
            // Kontrollera HTTP-statuskoden
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

            if (http_code == 200) {
                 std::cout << "<- Mottog 200 OK. Data skickad framgångsrikt." << std::endl;
            } else {
                 std::cerr << "<- Felaktig HTTP-statuskod: " << http_code << std::endl;
            }
        }

        // Rensa upp CURL-sessionen
        curl_easy_cleanup(curl);
    }
}

int main() {
    // Sätt startvärdet för slumptalsgeneratorn
    std::srand(std::time(nullptr));

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
