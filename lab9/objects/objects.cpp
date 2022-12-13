#include "objects.h"


AbstractCar::AbstractCar(int Tmax_fuel, float TrashodON100, float Tfuel_status) :
    odometr(0), max_fuel(Tmax_fuel), fuel_status(Tfuel_status), rashodON100(TrashodON100) {};

bool AbstractCar::drive (int kilometres) // пробег+, топливо-, проверка возможности езды(топливо/км)
{
    if (this->rashodON100/100*kilometres>this->fuel_status)
        return false;
    this->fuel_status -= this->rashodON100/100*kilometres;
    this->odometr += kilometres;
    return true;
};

void AbstractCar::refuel()  //до полного заправка
{
    this->fuel_status = float(this->max_fuel);
    cout << "Бак вашего ТС заправлен!\n";
};

void AbstractCar::printStatus() //общий пробег, остаток топлива
{
    cout << "Суммарный пробег вашего ТС: " << this->odometr << endl;
    cout << "Остаток топлива в баке вашего ТС: " << this->fuel_status << endl;
    int i = (this->fuel_status) / ((this->rashodON100) / 100);
    cout << "Примерный пробег до конца топлива: " << i << endl;
}

int AbstractCar::getMileage()
{
    return this->odometr;
}

float AbstractCar::getFuelStatus()
{
    return this->fuel_status;
};

AbstractCar::~AbstractCar()
{
    cout << "AbstractCar  - deleted.\n";
}

Sedan::Sedan() : AbstractCar(45, 7.0, 45.0) {};

Sedan::Sedan(int maxFuel, float rashod__100, float fuelStatus) :
    AbstractCar(maxFuel, rashod__100, fuelStatus) {};

Sedan::~Sedan()
{
    cout << "Sedan  - deleted.\n";
}

Suv::Suv() : AbstractCar(120, 11.5, 120.0) {};

Suv::Suv(int maxFuel, float rashod__100, float fuelStatus) :
    AbstractCar(maxFuel, rashod__100, fuelStatus) {};

Suv::~Suv()
{
    cout << "Suv  - deleted.\n";
}

Bus::Bus() : AbstractCar(450, 27.5, 450.0) {};

Bus::Bus(int maxFuel, float rashod__100, float fuelStatus) :
    AbstractCar(maxFuel, rashod__100, fuelStatus) {};

Bus::~Bus()
{
    cout << "Bus  - deleted.\n";
}

Bicycle::Bicycle() : IVehicle(), odometr(0) {};

Bicycle::~Bicycle()
{
    cout << "Bicycle  - deleted.\n";
}

bool Bicycle::drive (int kilometres)
{
    this->odometr += kilometres;
    return true;
}

void Bicycle::refuel()
{
    cout << "Ошибка! У велосипеда нет топливного бака.\n";
}

void Bicycle::printStatus()
{
    cout << "Суммарный пробег вашего ТС: " << this->odometr << endl;
}

int Bicycle::getMileage()
{
    return this->odometr;
}

RoutePoint Route::get_RPoint(int index)
{
    return this->road.at(index);
}

Route::Route(const RoutePoint& now_point)
{
    road.push_back(now_point);
}

Route::~Route()
{
    cout << "Деструктор маршрута отработал.\n";
}

void Route::addPoint(const RoutePoint& point)
{
    road.push_back(point);
}

bool Route::run(IVehicle* vehicle)
{
    if(road.size()<2)
    {
        cout << "Маршрут должен содержать как минимум 2 точки!\n";
        return false;
    }else {
        for (unsigned long long i = 0; i+1 < road.size(); ++i)
        {
            int kilometres = 0, dY = 0, dX = 0;
            RoutePoint first_point = road.at(i);
            RoutePoint second_point = road.at(i+1);
            dY = second_point.yKm - first_point.yKm;
            dX = second_point.xKm - first_point.xKm;
            kilometres = static_cast<int>(sqrt(pow(dY, 2) + pow(dX, 2)));
            if(!vehicle->drive(kilometres))
            {
                vehicle->refuel();
                cout << "//////////////////////\n";
                cout << "Произведена дозаправка\n";
                vehicle->printStatus();
                cout << "//////////////////////\n";
                if(!vehicle->drive(kilometres))
                {
                    cout << "ТС не может проехать такое расстояние без остановки! (" <<  kilometres << ")\n";
                    return false;
                }

            }
        }
    }
    return true;
}

