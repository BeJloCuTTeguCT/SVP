#include <iostream>
#include <fstream>
#include "../objects/objects.h"
using namespace std;

int main (int argc, char* argv[])
{   system("chcp 65001");
    if (argc < 2)
    {
        cout << "В качестве параметра передайте файл с точками маршрута.\n";
        exit(-1);
    }
    ifstream file(argv[1]);
    if(!file.is_open())
    {
        cout << "Ошибка открытия файла!\n";
        exit(-1);
    }
    Route route;
    while(!file.eof())
    {
        int koordinat[2];
        string name;
        file >> koordinat[0] >> koordinat[1] >> name;
        route.addPoint(RoutePoint{koordinat[0], koordinat[1], name});
    }

    IVehicle * vehicles[4] = {new Sedan, new Suv, new Bus, new Bicycle};
    for (int i = 0; i < 4; ++i)
    {
        IVehicle * vehicle = vehicles[i];
        cout << typeid(* vehicles[i]).name() << endl << "-----------------------------\n";
            cout << "Начало маршрута для данного ТС\n";
            route.run(vehicle);
            cout << "Конец маршрута для данного ТС\n";
            vehicle->printStatus();
            cout << "*********************************\n";
        delete vehicle;
        cout << endl << "___________________________" << endl;
    }
    cin.get();
    system("pause");
    return 0;
}
