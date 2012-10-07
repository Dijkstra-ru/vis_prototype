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
const size_t RVR = 2; // ����������� ������� �������
const size_t KOMPTENZORA = 4; // ���-�� ��������� �������
const size_t KUZLOV = 4; // ���������� ����� � ��������
const size_t KPEREMEL = KUZLOV*RVR; // ���-�� ���������� � ��������  �����  KUZLOV*RVR
const size_t MTOCHLINE = 2; // ����. ���. ��  ����� �������������� � ����� �� ��������
const size_t MTOCHEL = MTOCHLINE*MTOCHLINE; // ����. ���. ��  ����� �������������� �� �������� =MTOCHLINE*MTOCHLINE
const size_t KOLPRILELEM = 2;   // ������������ ���������� ����������� � ���������� ���� ���������

//namespace fs = boost::filesystem;




namespace col {
    using std::vector;
    using std::tr1::array;
}

//---------------------------------------------------------------------------
// ��������� ������ �� ������������
//----------------------------------------------------------------------------
struct nds_t
{// ��������� ���������� �� ��� ��������
  i64
    snpl, //��������� ����� ���� ����� �����������:
    // 1 - ���������, 2 - ������� � ��������, 3 - ������������
    snplVrem; //��������� ����� ���� � �������� ����������:
  // 1 - ���������, 2 - ������� � ��������, 3 - ������������,

  fp_t
    r, // ������ � ������ �������������� ��������
    jkobian, // �������
    s[KOMPTENZORA], //������ ������������������ ����������: ������ ������ -
    // ����� ����� �������������,  ������ ������ - ����������
    //�������: s[0] - Srr, s[1] - Syy, s[2] - Sff,  s[3] - Sry
    ss[KOMPTENZORA], // �������� ������� ����������
    SkorYprugNaprag[KOMPTENZORA],// �������� ������� ����������
    PrirashUprugixNapr[KOMPTENZORA], // ���������� ������� ����������
    Normal[KOMPTENZORA], // ������ ��������� ������� � ����������� ���������
    NaprVrem[KOMPTENZORA], // ��������� (�������) ����������
    DevNaprVrem[KOMPTENZORA], //�������� ������� ��������� ����������
    tsd[KOMPTENZORA], // ���������� ������� ��������� ����������
    sgrv[KOMPTENZORA], // ����� ������������ ��������� � ������������ �����������
    kss[KOMPTENZORA],//��������������� ������ � ��������
    sigma0, //������� ���������� ����������
    sigma0Vrem,
    tau,// ������������� ����������� ����������
    tauVrem,
    DlinaDevUprug, //=tau/sqrt(2)
    teta,  // ������������� ��������� ������
    tetaVrem,
    SkorTeta, // �������� �������������� ��������� ������
    spmu, //=2.*(met.lkmu*ndselem[ks].teta[j]+1.)
    LamdaPlast,  // ����� ������������,
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
    ak[KUZLOV], bk[KUZLOV], ck[KUZLOV], // ����������� ������� ����� ��
    //���������� ����������� � ��������
    MasPlast[KPEREMEL]; // ��������������� ������ ��� ������ ����� � ������������
public:
  nds_t();
};
//-----------------------------------------------------------------------

typedef col::array<nds_t, MTOCHEL> nds_in_elem;

struct fs_elem
{// ��������� ���������� � �������� ��������
  i64
    nuzlov[KUZLOV], // ���������� ������ ����� ���������
    nperemen[KPEREMEL],   // ���������� ������ ���������� � ����� ���������
    numVx[KUZLOV], // ���������� ������ ��� Vx (Vr) � ����� ��������
    numVy[KUZLOV], // ���������� ������ ��� Vy � ����� ��������
    SostoaniePlast; // 0 - ��� ����� �������� � ���������, 1 - ���� �� ���� �����
  // �������� � ��������, 2 - ��� ����� �������� � ��������
  fp_t
    x[KUZLOV],  // ���������� ���������� X ����� ��������
    y[KUZLOV];  // ���������� ���������� Y ����� ��������
public:
  fs_elem();
};

struct finite_el_t {
  u64 el_num;
  fs_elem el;
  nds_in_elem nds;
};


