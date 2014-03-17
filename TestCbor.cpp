// Copyright 2014 Jakob Varmose Bentzen | Released under the MIT License
#include "TestCbor.h"
#include "Cbor.h"

void TestCbor::encodePositiveInts () {
	QCOMPARE (Cbor (20).save (), QByteArray::fromHex ("14"));
	QCOMPARE (Cbor (200).save (), QByteArray::fromHex ("18c8"));
}
void TestCbor::decodePositiveInts () {
	QCOMPARE (cborLoad (QByteArray::fromHex ("14")), Cbor (20));
	QCOMPARE (cborLoad (QByteArray::fromHex ("18c8")), Cbor (200));
}
void TestCbor::encodeNegativeInts () {
	QCOMPARE (Cbor (-20).save (), QByteArray::fromHex ("33"));
	QCOMPARE (Cbor (-200).save (), QByteArray::fromHex ("38c7"));
}
void TestCbor::decodeNegativeInts () {
	QCOMPARE (cborLoad (QByteArray::fromHex ("33")), Cbor (-20));
	QCOMPARE (cborLoad (QByteArray::fromHex ("38c7")), Cbor (-200));
}
void TestCbor::encodeByteStrings () {
	QCOMPARE (Cbor (QByteArray ()).save (), QByteArray::fromHex ("40"));
	QCOMPARE (Cbor (QByteArray::fromHex ("010203")).save (), QByteArray::fromHex ("43010203"));
}
void TestCbor::decodeByteStrings () {
	QCOMPARE (cborLoad (QByteArray::fromHex ("40")), Cbor (QByteArray ()));
	QEXPECT_FAIL ("", "Indefinite length byte strings not supported", Continue);
	QCOMPARE (cborLoad (QByteArray::fromHex  ("5fff")), Cbor (QByteArray ()));
}
void TestCbor::encodeTextStrings () {
	QCOMPARE (Cbor ("").save (), QByteArray::fromHex ("60"));
	QCOMPARE (Cbor ("foo").save (), QByteArray::fromHex ("63666f6f"));
}
void TestCbor::decodeTextStrings () {
	QCOMPARE (cborLoad (QByteArray::fromHex ("60")), Cbor (""));
	QEXPECT_FAIL ("", "Indefinite length text strings not supported", Continue);
	QCOMPARE (cborLoad (QByteArray::fromHex  ("7fff")), Cbor (""));
}
void TestCbor::encodeArrays () {
	QCOMPARE (Cbor (CborList ()).save (), QByteArray::fromHex ("80"));
}
void TestCbor::decodeArrays () {
	QCOMPARE (cborLoad (QByteArray::fromHex ("80")), Cbor (CborList ()));
	QEXPECT_FAIL ("", "Indefinite length arrays not supported", Continue);
	QCOMPARE (cborLoad (QByteArray::fromHex ("9fff")), Cbor (CborList ()));
}
void TestCbor::encodeMaps () {
	QCOMPARE (Cbor (CborMap ()).save (), QByteArray::fromHex ("a0"));
}
void TestCbor::decodeMaps () {
	QCOMPARE (cborLoad (QByteArray::fromHex ("a0")), Cbor (CborMap ()));
	QEXPECT_FAIL ("", "Indefinite length maps not supported", Continue);
	QCOMPARE (cborLoad (QByteArray::fromHex ("bfff")), Cbor (CborMap ()));
}
void TestCbor::encodeTags () {
	QCOMPARE (Cbor (55799, Cbor ()).save (), QByteArray::fromHex ("d9d9f7f7"));
}
void TestCbor::decodeTags () {
	QCOMPARE (cborLoad (QByteArray::fromHex ("d9d9f7f7")), Cbor (55799, Cbor ()));
}
void TestCbor::encodeSimpleValues () {
	QCOMPARE (Cbor (false).save (), QByteArray::fromHex ("f4"));
	QCOMPARE (Cbor (true).save (), QByteArray::fromHex ("f5"));
	QCOMPARE (Cbor (Cbor::Null).save (), QByteArray::fromHex ("f6"));
	QCOMPARE (Cbor ().save (), QByteArray::fromHex ("f7"));
}
void TestCbor::decodeSimpleValues () {
	QCOMPARE (cborLoad (QByteArray::fromHex ("f4")), Cbor (false));
	QCOMPARE (cborLoad (QByteArray::fromHex ("f5")), Cbor (true));
	QCOMPARE (cborLoad (QByteArray::fromHex ("f6")), Cbor (Cbor::Null));
	QCOMPARE (cborLoad (QByteArray::fromHex ("f7")), Cbor ());
}
void TestCbor::encodeFloats () {
	QEXPECT_FAIL ("", "Half not supported", Continue);
	QCOMPARE (Cbor (0.0).save (), QByteArray::fromHex ("f90000"));
}
void TestCbor::decodeFloats () {
	QEXPECT_FAIL ("", "Half not supported", Continue);
	QCOMPARE (cborLoad (QByteArray::fromHex ("f90000")), Cbor (0.0));
}
void TestCbor::decodeInvalid () {
	QCOMPARE (Cbor ().load (QByteArray::fromHex ("")), false);
	QCOMPARE (Cbor ().load (QByteArray::fromHex ("1f")), false);
	QCOMPARE (Cbor ().load (QByteArray::fromHex ("18")), false);
	QCOMPARE (Cbor ().load (QByteArray::fromHex ("41")), false);
	QCOMPARE (Cbor ().load (QByteArray::fromHex ("61")), false);
	QCOMPARE (Cbor ().load (QByteArray::fromHex ("81")), false);
	QCOMPARE (Cbor ().load (QByteArray::fromHex ("b1")), false);
}

QTEST_MAIN (TestCbor)
