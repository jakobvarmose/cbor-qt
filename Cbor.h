// Copyright 2014 Jakob Varmose Bentzen | Released under the MIT License
#pragma once
#include <QList>
#include <QMap>
#include <QString>
#include <QDateTime>
class Cbor;
typedef QList<Cbor> CborList;
typedef QMap<Cbor,Cbor> CborMap;
typedef quint64 CborTag;
typedef QList<CborTag> CborTagList;
class Cbor {
public:
	enum Simple {
		False = 20,
		True,
		Null,
		Undefined
	};
	enum Type {
		UNSIGNED,
		NEGATIVE,
		BLOB,
		TEXT,
		ARRAY,
		MAP,
		SIMPLE,
		FLOAT
	};
	Cbor (unsigned int);
	Cbor (unsigned long long);
	Cbor (int);
	Cbor (long long);
	Cbor (const QByteArray &);
	Cbor (const char *);
	Cbor (const QString &);
	Cbor (const CborList &);
	Cbor (const CborMap &);
	Cbor (CborTag tag, const Cbor &);
	Cbor (Simple val = Cbor::Undefined);
	Cbor (bool);
	Cbor (double);
	Cbor (const QDateTime &);
	
	// Type
	Cbor::Type type () const;
	
	// Unsigned
	unsigned int toUInt (unsigned int def = 0) const;
	unsigned long long toULong (unsigned long long def = 0) const;
	
	// Signed
	int toInt (int def = 0) const;
	long long toLong (long long def = 0) const;
	
	// Blob
	QByteArray toByteArray (const QByteArray &def = QByteArray ()) const;
	
	// String
	QString toString (const QString &def = QString ()) const;
	
	// Array
	CborList toList (const CborList &def = CborList ()) const;
	
	// Map
	CborMap toMap (const CborMap &def = CborMap ()) const;
	
	// Tags
	CborTag tag () const;
	Cbor value () const;
	
	// Simple
	Cbor::Simple toSimple (Cbor::Simple def = Undefined) const;
	
	// Bool
	bool toBool (bool def = false) const;
	
	// Float
	double toDouble (double def = 0) const;
	
	// DateTime
	QDateTime toDateTime (const QDateTime & = QDateTime ()) const;
	
	QByteArray save () const;
	void write (QByteArray &) const;
	bool load (const QByteArray &);
	bool read (QByteArray &);
	bool read (const QByteArray &, quint64 &);
	
	bool operator < (const Cbor &other) const;
	bool operator == (const Cbor &other) const;
	bool operator != (const Cbor &other) const;
	
	QString toDebugString () const;
private:
	bool readVar (const QByteArray &input, quint64 &pos, int &major, int &minor, quint64 &size) const;
	void writeVar (QByteArray &result, int type, quint64 value) const;
	void write8 (QByteArray &result, int type, quint64 value) const;
	void write16 (QByteArray &result, int type, quint64 value) const;
	void write32 (QByteArray &result, int type, quint64 value) const;
	void write64 (QByteArray &result, int type, quint64 value) const;
	Type m_type;
	union {
		quint64 m_value;
		double m_float;
	};
	QString m_string;
	QByteArray m_blob;
	CborList m_list;
	CborMap m_map;
	CborTagList m_tags;
};
QByteArray cborSave (const Cbor &val);
Cbor cborLoad (const QByteArray &val);
