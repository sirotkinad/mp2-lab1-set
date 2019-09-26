// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0)
		throw "Incorrect data";
	else {
		BitLen = len;
		if ((len % (sizeof(TELEM) * 8) == 0))
			MemLen = len / (sizeof(TELEM) * 8);
		else 
		{
			MemLen = len / (sizeof(TELEM) * 8) + 1;
		}
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	pMem = new TELEM[bf.MemLen];
	MemLen = bf.MemLen;
	for (int i = 0; i < bf.MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n >= 0 && n < BitLen)
	{
		return (n / (sizeof(TELEM) * 8));
	}
	else
		throw "Incorrect data";
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int mask;
	if (n < 0 || n >= BitLen)
		throw("Ошибка");
	mask = n - GetMemIndex(n) * (8 * sizeof(TELEM));
	return (1 << mask);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 && n < BitLen)
	{  
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	}
	else
		throw "Incorrect data";
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= 0 && n < BitLen) 
	{ 
		pMem[GetMemIndex(n)] &= (~GetMemMask(n));
	}
	else
		throw "Incorrect data";
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= 0 && n < BitLen) 
	{
		if ((pMem[GetMemIndex(n)] & GetMemMask(n)) == GetMemMask(n))
			return 1;
		else
			return 0;
	}
	else
		throw "Incorrect data";
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (BitLen != bf.BitLen)
	{
		BitLen = bf.BitLen;
		for (int i = 0; i < MemLen; i++)
			delete[] pMem;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int k = 0;
	if (MemLen == bf.MemLen) 
	{
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] == bf.pMem[i])
				k++;
		}
		if (k == MemLen)
			return 1;
	}
	else
		return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int k = 0;
	if (MemLen == bf.MemLen) 
	{
		for (int i = 0; i < MemLen; i++) {
			if (pMem[i] == bf.pMem[i])
				k++;
		}
		if (k == MemLen)
			return 0;
		else
			return 1;
	}
	else
		return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField SumField(len);
	for (int i = 0; i < MemLen; i++)
		SumField.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		SumField.pMem[i] |= bf.pMem[i];
	return SumField;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField MultField(len);
	for (int i = 0; i < MemLen; i++)
		MultField.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		MultField.pMem[i] &= bf.pMem[i];
	return MultField;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField InvertField(BitLen);
	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i) == 0)
			InvertField.SetBit(i);
		if (GetBit(i) == 1)
			InvertField.ClrBit(i);
	}
	return InvertField;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char symbol;
	for (int i = 0; i < bf.BitLen; i++) 
	{
		istr >> symbol;
		if ((int)symbol == 1)
			bf.SetBit(i);
		if (!(int)symbol)
			bf.ClrBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (bf.GetBit(i) == 1) 
		{
			ostr << "1";
		}
		else 
		{
			ostr << "0";
		}
	}
	return ostr;
}

