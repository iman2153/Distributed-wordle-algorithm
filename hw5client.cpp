#include <iostream>
#include "hw5client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>
#include "general.h"
#include "person.h"
#include "thing.h"
#include "time.h"
#include "gps.h"

using namespace jsonrpc;
using namespace std;

int progression = 0;

void init_server_location(hw5Client &client, gps location) {
  Json::Value update_info;
  update_info["Type"] = "update";
  update_info["location"] = location.dump2JSON();
  client.action(update_info);
}

void add_person_to_server(hw5Client &client, person p) {
  Json::Value update_info;
  Json::Value persons;
  persons[0] = p.dump2JSON();
  update_info["Type"] = "update";
  update_info["persons"] = persons;
  client.action(update_info);
}

void progress_time(vector<hw5Client*> clients) {
  progression++;
  for(int i = 0; i < clients.size(); i++) {
    Json::Value update_info;
    update_info["Type"] = "update";
    update_info["time"] = _time(progression).dump2JSON();
    clients[i]->action(update_info);
  }
}

void print_servers_info(vector<hw5Client*> clients) {
  for(int i = 0; i < clients.size(); i++) {
    Json::Value request;
    Json::Value server_info = clients[i]->action(request);
    std::cout << server_info["location"]["name"].asString() << "'s info:" << std::endl;
    std::cout << server_info << std::endl;
  }
}

void transfer_thing(hw5Client &client, string from_person, string to_person, thing thing) {
  Json::Value transfer_info;
  transfer_info["Type"] = "transfer_thing";
  transfer_info["from_person"] = from_person;
  transfer_info["to_person"] = to_person;
  transfer_info["thing"] = thing.dump2JSON();
  client.action(transfer_info);
}

void move(string name, hw5Client &from, hw5Client &to) {
  Json::Value move_info;
  Json::Value persons;
  persons[0] = name;
  move_info["persons"] = persons;

  move_info["Type"] = "move_from";
  move_info = from.action(move_info);

  move_info["Type"] = "move_to";
  move_info = to.action(move_info);
}

int main() {
    HttpClient httpclient1("http://127.0.0.1:8001");
    hw5Client hw5Home(httpclient1, JSONRPC_CLIENT_V2);
    HttpClient httpclient2("http://127.0.0.1:8002");
    hw5Client hw5Forest(httpclient2, JSONRPC_CLIENT_V2);
    HttpClient httpclient3("http://127.0.0.1:8003");
    hw5Client hw5GrandmaHome(httpclient3, JSONRPC_CLIENT_V2);
    gps Home_location("Home", 0, 0);
    gps Forest_location("Forest", 5, 5);
    gps GrandmaHome_location("GrandmaHome", 10, 10);
    init_server_location(hw5Home, Home_location);
    init_server_location(hw5Forest, Forest_location);
    init_server_location(hw5GrandmaHome, GrandmaHome_location);
    vector<hw5Client*> clients;
    clients.push_back(&hw5Home);
    clients.push_back(&hw5Forest);
    clients.push_back(&hw5GrandmaHome);

    //Initialize objects
    thing cap = thing("Cap", "a little cap of red velvet");
    thing cake = thing("Cake", "a piece of cake");
    thing wine = thing("Wine", "a bottle of wine");
    person littleRedRidingHood;
    littleRedRidingHood.name = "Little Red Riding Hood";
    littleRedRidingHood.gender = "female";
    littleRedRidingHood.age = 8;
    littleRedRidingHood.location = Home_location;
    littleRedRidingHood.gainThing(cap);
    person Mom;
    Mom.name = "Mom";
    Mom.gender = "female";
    Mom.age = 30;
    Mom.location = Home_location;
    Mom.gainThing(cake);
    Mom.gainThing(wine);
    person grandma;
    grandma.name = "Grandma";
    grandma.gender = "female";
    grandma.age = 70;
    grandma.location = GrandmaHome_location;
    person wolf;
    wolf.name = "Wolf";
    wolf.gender = "male";
    wolf.age = 20;
    wolf.location = Forest_location;

    //Put objects on servers
    add_person_to_server(hw5Home, littleRedRidingHood);
    add_person_to_server(hw5Home, Mom);
    add_person_to_server(hw5Forest, wolf);
    add_person_to_server(hw5GrandmaHome, grandma);
    
    //Event1
    std::cout << "Hit enter to progress events" << endl;
    getchar();
    progress_time(clients);
    std::cout << "Event 1 occurs:" << endl;
    print_servers_info(clients);

    //Event2
    std::cout << "Hit enter to progress events" << endl;
    getchar();
    transfer_thing(hw5Home, "Mom", "Little Red Riding Hood", cake);
    transfer_thing(hw5Home, "Mom", "Little Red Riding Hood", wine);
    progress_time(clients);
    std::cout << "Event 2 occurs" << endl;
    print_servers_info(clients);

    //Event3
    std::cout << "Hit enter to progress events" << endl;
    getchar();
    move("Little Red Riding Hood", hw5Home, hw5Forest);
    progress_time(clients);
    std::cout << "Event 3 occurs" << endl;
    print_servers_info(clients);

    //Event4
    std::cout << "Hit enter to progress events" << endl;
    getchar();
    progress_time(clients);
    std::cout << "Event 4 occurs" << endl;
    print_servers_info(clients);
    std::cout << "All events finished" << endl;
    
    return 0;
}