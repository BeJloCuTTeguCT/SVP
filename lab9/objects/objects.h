#ifndef OBJECTS_H
#define OBJECTS_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class IVehicle //интерфейс не может иметь свойств. только неопределенные методы.
{
public:
    virtual bool drive (int kilometres) = 0;
    virtual void refuel() = 0;
    virtual void printStatus() = 0;
    virtual int getMileage() = 0;
};

class AbstractCar : public IVehicle
{
    int odometr;
    int max_fuel;
    float fuel_status;
    float rashodON100;

public:
    AbstractCar(int Tmax_fuel = 75, float TrashodON100 = 6.5, float Tfuel_status = 75);
    bool drive (int kilometres) override; // пробег+, топливо-, проверка возможности езды(топливо/км)
    void refuel() override;  //до полного заправка
    void printStatus() override; //общий пробег, остаток топлива
    int getMileage() override;
    float getFuelStatus();
    ~AbstractCar();
};

class Sedan : public AbstractCar
{
public:
    Sedan();
    Sedan(int maxFuel, float rashod__100, float fuelStatus);
    ~Sedan();
};

class Suv : public AbstractCar
{
public:
    Suv();
    Suv(int maxFuel, float rashod__100, float fuelStatus);
    ~Suv();
};

class Bus : public AbstractCar
{
public:
    Bus();
    Bus(int maxFuel, float rashod__100, float fuelStatus);
    ~Bus();
};

class Bicycle : public IVehicle
{
    int odometr;
public:
    Bicycle();
    ~Bicycle();
    bool drive (int kilometres) override;
    void refuel() override;
    void printStatus() override;
    int getMileage() override;
};

struct RoutePoint
{
    int xKm;
    int yKm;
    string name;
};

class Route
{
    vector <RoutePoint> road;

public:
    Route(){road.clear();};
    Route(const RoutePoint& point);
    void addPoint(const RoutePoint& point); //добавление точки в маршрут
    bool run(IVehicle* vehicle); //начало движения по маршруту
    RoutePoint get_RPoint(int index);
    ~Route();
};


#endif // OBJECTS_H
