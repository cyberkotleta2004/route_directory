#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <map>

using namespace std;

string generate_random_name(mt19937& gen, const string& prefix, int index) {
    static const vector<string> locations = {
        "вокзал", "площадь", "мост", "район", "аэропорт", "парк", "улица", "проспект", "бульвар"
    };
    static const vector<string> adjectives = {
        "Центральная", "Северный", "Южный", "Западный", "Восточный", 
        "Ривьерский", "Морской", "Железнодорожный", "Главный", "Старый"
    };
    
    if (gen() % 2 == 0) {
        return adjectives[index % adjectives.size()] + " " + locations[index % locations.size()];
    } else {
        return adjectives[index % adjectives.size()] + " " + locations[index % locations.size()] + " " + to_string(index % 10 + 1);
    }
}

void generate_json_file(const string& filename, int stop_count = 50, int bus_count = 30, int stat_count = 2000) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to create file: " << filename << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> lat_dist(43500000, 43650000); // 43.500000 - 43.650000
    uniform_int_distribution<> lon_dist(39600000, 39800000); // 39.600000 - 39.800000
    uniform_int_distribution<> dist_dist(500, 5000); // расстояния между остановками
    
    // Генерируем остановки
    vector<string> stop_names;
    for (int i = 0; i < stop_count; ++i) {
        stop_names.push_back(generate_random_name(gen, "Stop", i));
    }

    file << "{\n";
    file << "  \"base_requests\": [\n";

    // Генерация остановок (Stop)
    for (int i = 0; i < stop_count; ++i) {
        file << "    {\n";
        file << "      \"type\": \"Stop\",\n";
        file << "      \"name\": \"" << stop_names[i] << "\",\n";
        file << "      \"latitude\": " << lat_dist(gen) / 1000000.0 << ",\n";
        file << "      \"longitude\": " << lon_dist(gen) / 1000000.0 << ",\n";
        file << "      \"road_distances\": {\n";

        // Создаем связи с 1-3 другими случайными остановками
        int connections = gen() % 3 + 1;
        vector<int> connected;
        for (int j = 0; j < connections; ++j) {
            int target;
            do {
                target = gen() % stop_count;
            } while (target == i || find(connected.begin(), connected.end(), target) != connected.end());
            
            connected.push_back(target);
            file << "        \"" << stop_names[target] << "\": " << dist_dist(gen);
            if (j < connections - 1) file << ",";
            file << "\n";
        }

        file << "      }\n";
        file << (i < stop_count - 1 || bus_count > 0 ? "    },\n" : "    }\n");
    }

    // Генерация маршрутов (Bus)
    for (int i = 0; i < bus_count; ++i) {
        file << "    {\n";
        file << "      \"type\": \"Bus\",\n";
        file << "      \"name\": \"" << to_string(100 + i) << "\",\n";
        
        // Выбираем случайные остановки для маршрута (2-10 остановок)
        int stops_in_route = gen() % 9 + 2;
        vector<int> route_stops;
        for (int j = 0; j < stops_in_route; ++j) {
            route_stops.push_back(gen() % stop_count);
        }

        file << "      \"stops\": [\n";
        for (int j = 0; j < stops_in_route; ++j) {
            file << "        \"" << stop_names[route_stops[j]] << "\"";
            if (j < stops_in_route - 1) file << ",";
            file << "\n";
        }
        file << "      ],\n";
        
        bool is_roundtrip = gen() % 2 == 0;
        file << "      \"is_roundtrip\": " << (is_roundtrip ? "true" : "false") << "\n";
        file << (i < bus_count - 1 ? "    },\n" : "    }\n");
    }

    file << "  ],\n";
    file << "  \"stat_requests\": [\n";

    // Генерация stat_requests
    for (int i = 0; i < stat_count; ++i) {
        bool is_bus = gen() % 2 == 0;
        if (is_bus && bus_count > 0) {
            file << "    { \"id\": " << i + 1 << ", \"type\": \"Bus\", \"name\": \"" << to_string(100 + gen() % bus_count) << "\" }";
        } else {
            file << "    { \"id\": " << i + 1 << ", \"type\": \"Stop\", \"name\": \"" << stop_names[gen() % stop_count] << "\" }";
        }
        file << (i < stat_count - 1 ? ",\n" : "\n");
    }

    file << "  ]\n";
    file << "}\n";

    cout << "Generated JSON file: " << filename << endl;
    cout << "Stops: " << stop_count << ", Buses: " << bus_count << ", Stat requests: " << stat_count << endl;
}

// int main() {
//     generate_json_file("tests/json_test_files/big_data.json", 1000, 1000, 10000);
//     return 0;
// }