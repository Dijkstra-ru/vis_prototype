#pragma once
#include <vector>
#include <array>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <type_traits>
#include <iostream>
#include <stdint.h>


typedef double fp_t; //typedef for ease of changes

typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef uint8_t u8;

namespace col {
    using std::vector;
    using std::tr1::array;
};


//---------------------------------------------------------------------------
const size_t RVR = 2; // размерность вектора решений
const size_t KOMPTENZORA = 4; // кол-во компонент тензора
const size_t KUZLOV = 4; // количество узлов в элементе
const size_t KPEREMEL = KUZLOV*RVR; // кол-во переменных в элементе  равно  KUZLOV*RVR
const size_t MTOCHLINE = 2; // макс. кол. во  точек интегрирования в линии на элементе
const size_t MTOCHEL = MTOCHLINE*MTOCHLINE; // макс. кол. во  точек интегрирования на элементе =MTOCHLINE*MTOCHLINE
const size_t KOLPRILELEM = 2;   // максимальное количество прилегающих к граничному узлу элементов

//namespace fs = boost::filesystem;




namespace col {
    using std::vector;
    using std::tr1::array;
}

//---------------------------------------------------------------------------
// структуры данных от пользователя
//----------------------------------------------------------------------------
struct nds_t
{// структура информации об НДС элемента
  i64
    snpl, //состояние точки тела перед деформацией:
    // 1 - упругость, 2 - переход в пластику, 3 - пластичность
    snplVrem; //состояние точки тела в процессе деформации:
  // 1 - упругость, 2 - переход в пластику, 3 - пластичность,

  fp_t
    r, // радиус в точках интегрирования элемента
    jkobian, // якобиан
    s[KOMPTENZORA], //тензор упругопластических напряжений: первый индекс -
    // номер точки нтегрирования,  второй индекс - компонента
    //тензора: s[0] - Srr, s[1] - Syy, s[2] - Sff,  s[3] - Sry
    ss[KOMPTENZORA], // девиатор тензора напряжений
    SkorYprugNaprag[KOMPTENZORA],// скорость упругих напряжений
    PrirashUprugixNapr[KOMPTENZORA], // приращение упругих напряжений
    Normal[KOMPTENZORA], // тензор единичной нормали к поверхности текучести
    NaprVrem[KOMPTENZORA], // временные (упругие) напряжения
    DevNaprVrem[KOMPTENZORA], //девиатор тензора временных напряжений
    tsd[KOMPTENZORA], // компоненты тензора скоростей деформаций
    sgrv[KOMPTENZORA], // сумма конвективных слагаемых в определяющем соотношении
    kss[KOMPTENZORA],//вспомогательный массив в пластике
    sigma0, //среднее нормальное напряжение
    sigma0Vrem,
    tau,// интенсивность касательных напряжений
    tauVrem,
    DlinaDevUprug, //=tau/sqrt(2)
    teta,  // относительное изменение объема
    tetaVrem,
    SkorTeta, // скорость относительного изменения объема
    spmu, //=2.*(met.lkmu*ndselem[ks].teta[j]+1.)
    LamdaPlast,  // Ламда пластическая,
    SkorLamdaPlast,
    sklamda,
    PrirashLamdaPlast,
    PrirashLamdaPlastStariy,
    kn,
    knVrem,
    dKndLamda,
    p2,
    h,
    SkorKn,
    ak[KUZLOV], bk[KUZLOV], ck[KUZLOV], // Производные функции формы по
    //глобальным координатам в элементе
    MasPlast[KPEREMEL]; // вспомогательный массив для правой части в пластичности
public:
  nds_t();
};
//-----------------------------------------------------------------------

typedef col::array<nds_t, MTOCHEL> nds_in_elem;

struct fs_elem
{// структура информации о конечном элементе
  i64
    nuzlov[KUZLOV], // глобальные номера узлов элементов
    nperemen[KPEREMEL],   // глобальные номера переменных в узлах элементов
    numVx[KUZLOV], // глобальные номера для Vx (Vr) в узлах элемента
    numVy[KUZLOV], // глобальные номера для Vy в узлах элемента
    SostoaniePlast; // 0 - все точки элемента в упругости, 1 - хотя бы одна точка
  // элемента в пластике, 2 - все точки элемента в пластике
  fp_t
    x[KUZLOV],  // глобальная координата X узлов элемента
    y[KUZLOV];  // глобальная координата Y узлов элемента
public:
  fs_elem();
};

struct finite_el_t {
  u64 el_num;
  fs_elem el;
  nds_in_elem nds;
};


