#include <QtTest>
#include "../objects/objects.h"

class test_lab5 : public QObject
{
    Q_OBJECT

public:
    test_lab5();
    ~test_lab5();

private slots:
    void test_Sedan();
    void test_Bus();
    void test_Suv();
    void test_Bicycle();
    void test_RoutePoints();
    void test_Route();
};

test_lab5::test_lab5()
{

}

test_lab5::~test_lab5()
{

}

void test_lab5::test_Sedan()
{
    Sedan sedan(50, 7, 50); //первый конструктор
    QCOMPARE(sedan.getMileage(), 0); //проверка пробега после инициализации
    QCOMPARE(sedan.getFuelStatus(), 50.0); //проверка полного бака после инициализации
    QVERIFY(sedan.drive(714)); // максимальное расстояние без дробных при баке 50 и расходе 7
    QVERIFY(!sedan.drive(1)); // уже не сможет проехать
    QVERIFY(sedan.getFuelStatus() < 0.03 && 0.02 < sedan.getFuelStatus()); //проверка расхода топлива
    QCOMPARE(sedan.getMileage(), 714); //проверка пробега
    sedan.refuel(); //заправка до полного
    QCOMPARE(sedan.getFuelStatus(), 50.0); //проверка заправки

    Sedan sedan2; //второй конструктор
    QCOMPARE(sedan2.getMileage(), 0); //проверка пробега после инициализации
    QCOMPARE(sedan2.getFuelStatus(), 45); //проверка уровня топлива после инициализации
}

void test_lab5::test_Suv()
{
    Suv suv(50, 7, 50); //первый конструктор
    QCOMPARE(suv.getMileage(), 0); //проверка пробега после инициализации
    QCOMPARE(suv.getFuelStatus(), 50.0); //проверка полного бака после инициализации
    QVERIFY(suv.drive(714)); // максимальное расстояние без дробных при баке 50 и расходе 7
    QVERIFY(!suv.drive(1)); // уже не сможет проехать
    QVERIFY(suv.getFuelStatus() < 0.03 && 0.02 < suv.getFuelStatus()); //проверка расхода топлива
    QCOMPARE(suv.getMileage(), 714); //проверка пробега
    suv.refuel(); //заправка до полного
    QCOMPARE(suv.getFuelStatus(), 50.0); //проверка заправки

    Suv suv2; //второй конструктор
    QCOMPARE(suv2.getMileage(), 0); //проверка пробега после инициализации
    QCOMPARE(suv2.getFuelStatus(), 120); //проверка уровня топлива после инициализации
}

void test_lab5::test_Bus()
{
    Bus bus(50, 7, 50); //первый конструктор
    QCOMPARE(bus.getMileage(), 0); //проверка пробега после инициализации
    QCOMPARE(bus.getFuelStatus(), 50.0); //проверка полного бака после инициализации
    QVERIFY(bus.drive(714)); // максимальное расстояние без дробных при баке 50 и расходе 7
    QVERIFY(!bus.drive(1)); // уже не сможет проехать
    QVERIFY(bus.getFuelStatus() < 0.03 && 0.02 < bus.getFuelStatus()); //проверка расхода топлива
    QCOMPARE(bus.getMileage(), 714); //проверка пробега
    bus.refuel(); //заправка до полного
    QCOMPARE(bus.getFuelStatus(), 50.0); //проверка заправки

    Bus bus2; //второй конструктор
    QCOMPARE(bus2.getMileage(), 0); //проверка пробега после инициализации
    QCOMPARE(bus2.getFuelStatus(), 450); //проверка уровня топлива после инициализации
}

void test_lab5::test_Bicycle()
{
    Bicycle bicycle; //первый конструктор
    QCOMPARE(bicycle.getMileage(), 0); //проверка пробега после инициализации
    QVERIFY(bicycle.drive(1)); // не ограничен в максимальном пробеге
    QVERIFY(bicycle.drive(10)); // не ограничен в максимальном пробеге
    QVERIFY(bicycle.drive(100)); // не ограничен в максимальном пробеге
    QVERIFY(bicycle.drive(1000)); // не ограничен в максимальном пробеге
    QVERIFY(bicycle.drive(10000)); // не ограничен в максимальном пробеге
    QCOMPARE(bicycle.getMileage(), 11111); //проверка пробега
}

void test_lab5::test_RoutePoints()
{
    RoutePoint rp{1, 5, "Point1"};

    QCOMPARE(rp.xKm, 1);
    QCOMPARE(rp.yKm, 5);
    QCOMPARE(rp.name, "Point1");
}

void test_lab5::test_Route()
{
    RoutePoint rp1{0, 0, "Point1"};
    RoutePoint rp2{100, 0, "Point2"};
    RoutePoint rp3{200, 100, "Point3"};
    RoutePoint rp4{300, 300, "Point4"};
    RoutePoint rp5{100, 400, "Point5"};
    RoutePoint rp6{0, 450, "Point6"};

    Suv suv;

    Route rout;
    QVERIFY_EXCEPTION_THROWN(rout.get_RPoint(0), std::out_of_range); // проверка пустоты массива точек дороги
    rout.addPoint(rp1); //добавление 1 точки дороги
    QVERIFY_EXCEPTION_THROWN(rout.get_RPoint(1), std::out_of_range); // проверка добавления 1 точки
    QVERIFY(!rout.run(&suv)); //маршрут должен состоять мин из 2х точек
    rout.addPoint(rp2); //добавление 2й точки дороги
    QVERIFY_EXCEPTION_THROWN(rout.get_RPoint(2), std::out_of_range); // проверка добавления 1 точки
    QVERIFY(rout.run(&suv)); //маршрут должен состоять мин из 2х точек
    rout.addPoint(rp3); //добавление 2й точки дороги
    QVERIFY_EXCEPTION_THROWN(rout.get_RPoint(3), std::out_of_range); // проверка добавления 1 точки
    QVERIFY(rout.run(&suv));
    rout.addPoint(rp4); //добавление 2й точки дороги
    QVERIFY_EXCEPTION_THROWN(rout.get_RPoint(4), std::out_of_range); // проверка добавления 1 точки
    QVERIFY(rout.run(&suv));
    rout.addPoint(rp5); //добавление 2й точки дороги
    QVERIFY_EXCEPTION_THROWN(rout.get_RPoint(5), std::out_of_range); // проверка добавления 1 точки
    QVERIFY(rout.run(&suv));
    rout.addPoint(rp6); //добавление 2й точки дороги
    QVERIFY_EXCEPTION_THROWN(rout.get_RPoint(6), std::out_of_range); // проверка добавления 1 точки
    QVERIFY(rout.run(&suv)); //маршрут должен состоять мин из 2х точек
}



QTEST_APPLESS_MAIN(test_lab5)

#include "tst_test_lab5.moc"
