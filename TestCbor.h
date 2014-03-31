// Copyright 2014 Jakob Varmose Bentzen | Released under the MIT License
#pragma once
#include <QtTest/QtTest>

class TestCbor : public QObject {
	Q_OBJECT
private slots:
	void encodePositiveInts ();
	void decodePositiveInts ();
	
	void encodeNegativeInts ();
	void decodeNegativeInts ();
	
	void encodeByteStrings ();
	void decodeByteStrings ();
	
	void encodeTextStrings ();
	void decodeTextStrings ();
	
	void encodeArrays ();
	void decodeArrays ();
	
	void encodeMaps ();
	void decodeMaps ();
	
	void encodeTags ();
	void decodeTags ();
	
	void encodeSimpleValues ();
	void decodeSimpleValues ();
	
	void encodeFloats ();
	void decodeFloats ();
	
	void decodeInvalid ();
	
	void debug ();
};
