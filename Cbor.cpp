// Copyright 2014 Jakob Varmose Bentzen | Released under the MIT License
#include "Cbor.h"
#include <stdexcept>
Cbor::Cbor (unsigned int val) {
	this->m_type = UNSIGNED;
	this->m_value = val;
}
Cbor::Cbor (unsigned long long val) {
	this->m_type = UNSIGNED;
	this->m_value = val;
}
Cbor::Cbor (int val) {
	if (val < 0) {
		this->m_type = NEGATIVE;
		this->m_value = -1 - val;
	} else {
		this->m_type = UNSIGNED;
		this->m_value = val;
	}
}
Cbor::Cbor (long long val) {
	if (val < 0) {
		this->m_type = Cbor::NEGATIVE;
		this->m_value = -1 - val;
	} else {
		this->m_type = Cbor::UNSIGNED;
		this->m_value = val;
	}
}
Cbor::Cbor (const QByteArray &value) {
	this->m_type = Cbor::BLOB;
	this->m_blob = value;
}
Cbor::Cbor (const char *value) {
	this->m_type = Cbor::TEXT;
	this->m_string = QString::fromUtf8 (value);
}
Cbor::Cbor (const QString &value) {
	this->m_type = Cbor::TEXT;
	this->m_string = value;
}
Cbor::Cbor (CborTag tag, const Cbor &other) {
	this->m_type = other.m_type;
	this->m_value = other.m_value;
	this->m_string = other.m_string;
	this->m_list = other.m_list;
	this->m_map = other.m_map;
	this->m_tags = other.m_tags;
	this->m_tags.push_front (tag);
}
Cbor::Cbor (bool value) {
	this->m_type = Cbor::SIMPLE;
	if (value == false) {
		this->m_value = Cbor::False;
	} else {
		this->m_value = Cbor::True;
	}
}
Cbor::Cbor (const CborList &val) : m_type (ARRAY), m_list (val) {
}
Cbor::Cbor (const CborMap &val) {
	this->m_type = MAP;
	this->m_map = val;
}
Cbor::Cbor (double value) : m_type (FLOAT), m_float (value) {
}
Cbor::Cbor (Cbor::Simple val) {
	this->m_type = SIMPLE;
	this->m_value = val;
}
Cbor::Cbor (const QDateTime &val) {
	if (val.isValid ()) {
		this->m_type = UNSIGNED;
		this->m_value = val.toTime_t ();
		this->m_tags.push_front (1);
	} else {
		this->m_type = SIMPLE;
		this->m_value = Cbor::Null;
	}
}
QDateTime Cbor::toDateTime (const QDateTime &def) const {
	switch (this->m_type) {
	case UNSIGNED:
		return QDateTime::fromTime_t (this->m_value);
	default:
		return def;
	}
}

double Cbor::toDouble (double def) const {
	switch (this->m_type) {
	case UNSIGNED:
		return this->m_value;
	case NEGATIVE:
		// TODO test this
		// this should avoid overflow and round correctly
		return (-1 - (this->m_value & 0xffffffff)) + double (- (this->m_value >> 32)) * (1ll << 32);
	case FLOAT:
		return this->m_float;
	default:
		return def;
	}
}
bool Cbor::toBool (bool def) const {
	switch (this->m_type) {
	case SIMPLE:
		if (this->m_value == Cbor::False) {
			return false;
		} else if (this->m_value == Cbor::True) {
			return true;
		} else {
			return def;
		}
	default:
		return def;
	}
}
unsigned int Cbor::toUInt (unsigned int def) const {
	switch (this->m_type) {
	case UNSIGNED:
		return this->m_value;
	case NEGATIVE:
		return -1 - this->m_value;
	case FLOAT:
		return this->m_float;
	default:
		return def;
	}
}
int Cbor::toInt (int def) const {
	switch (this->m_type) {
	case UNSIGNED:
		return this->m_value;
	case NEGATIVE:
		return -1 - this->m_value;
	case FLOAT:
		return this->m_float;
	default:
		return def;
	}
}
long long Cbor::toLong (long long def) const {
	switch (this->m_type) {
	case UNSIGNED:
		return this->m_value;
	case NEGATIVE:
		return -1 - this->m_value;
	case FLOAT:
		return this->m_float;
	default:
		return def;
	}
}
unsigned long long Cbor::toULong (unsigned long long def) const {
	switch (this->m_type) {
	case UNSIGNED:
		return this->m_value;
	case NEGATIVE:
		return -1 - this->m_value;
	case FLOAT:
		return this->m_float;
	default:
		return def;
	}
}
QByteArray Cbor::toByteArray (const QByteArray &def) const {
	if (this->m_type == BLOB) {
		return this->m_blob;
	} else {
		return def;
	}
}
QString Cbor::toString (const QString &def) const {
	if (this->m_type == TEXT) {
		return this->m_string;
	} else {
		return def;
	}
}
CborList Cbor::toList (const CborList &def) const {
	if (this->m_type == ARRAY) {
		return this->m_list;
	} else {
		return def;
	}
}
CborMap Cbor::toMap (const CborMap &def) const {
	if (this->m_type == MAP) {
		return this->m_map;
	} else {
		return def;
	}
}
Cbor::Simple Cbor::toSimple (Cbor::Simple def) const {
	if (this->m_type == Cbor::SIMPLE) {
		return (Cbor::Simple) this->m_value;
	} else {
		return def;
	}
}
Cbor::Type Cbor::type () const {
	return this->m_type;
}
void Cbor::write8 (QByteArray &result, int type, quint64 value) const {
	if (value < 24) {
		result.push_back (type << 5 | value);
	} else {
		result.push_back (type << 5 | 24);
		result.push_back (value);
	}
}
void Cbor::write16 (QByteArray &result, int type, quint64 value) const {
	result.push_back (type << 5 | 25);
	result.push_back (value >> 8);
	result.push_back (value);
}
void Cbor::write32 (QByteArray &result, int type, quint64 value) const {
	result.push_back (type << 5 | 26);
	result.push_back (value >> 24);
	result.push_back (value >> 16);
	result.push_back (value >> 8);
	result.push_back (value);
}
void Cbor::write64 (QByteArray &result, int type, quint64 value) const {
	result.push_back (type << 5 | 27);
	result.push_back (value >> 56);
	result.push_back (value >> 48);
	result.push_back (value >> 40);
	result.push_back (value >> 32);
	result.push_back (value >> 24);
	result.push_back (value >> 16);
	result.push_back (value >> 8);
	result.push_back (value);
}
void Cbor::writeVar (QByteArray &result, int type, quint64 value) const {
	if (value < 1ull << 8) {
		this->write8 (result, type, value);
	} else if (value < 1ull << 16) {
		this->write16 (result, type, value);
	} else if (value < 1ull << 32) {
		this->write32 (result, type, value);
	} else {
		this->write64 (result, type, value);
	}
}
CborTag Cbor::tag () const {
	return this->m_tags.front ();
}
Cbor Cbor::value () const {
	Cbor res (*this);
	res.m_tags.pop_front ();
	return res;
}
void Cbor::write (QByteArray &result) const {
	foreach (quint64 tag, this->m_tags) {
		this->writeVar (result, 6, tag);
	}
	switch (this->m_type) {
	case UNSIGNED:
	case NEGATIVE:
		writeVar (result, this->m_type, this->m_value);
		break;
	case BLOB: {
		writeVar (result, this->m_type, this->m_blob.size ());
		result.append (this->m_blob);
		break;
	}
	case TEXT: {
		writeVar (result, this->m_type, this->m_string.size ());
		result.append (this->m_string.toUtf8 ());
		break;
	}
	case ARRAY:
		writeVar (result, this->m_type, this->m_list.size ());
		foreach (const Cbor &child, this->m_list) {
			child.write (result);
		}
		break;
	case MAP:
		writeVar (result, this->m_type, this->m_map.size ());
		foreach (const Cbor &key, this->m_map.keys ()) {
			foreach (const Cbor &child, this->m_map.values (key)) {
				key.write (result);
				child.write (result);
			}
		}
		break;
	case SIMPLE:
		write8 (result, 7, this->m_value);
		break;
	case FLOAT: {
		if (this->m_float == (double)(float)this->m_float) {
			union {
				float f;
				quint32 i;
			};
			f = this->m_float;
			write32 (result, 7, i);
		} else {
			write64 (result, 7, this->m_value);
		}
		break;
	}
	}
}
QByteArray Cbor::save () const {
	QByteArray buffer;
	this->write (buffer);
	return buffer;
}
bool Cbor::load (const QByteArray &buffer) {
	quint64 pos = 0;
	bool res = this->read (buffer, pos);
	return res;
}
bool Cbor::read (QByteArray &buffer) {
	quint64 pos = 0;
	if (!this->read (buffer, pos)) {
		return false;
	}
	buffer.remove (0, pos);
	return true;
}

bool Cbor::readVar (const QByteArray &input, quint64 &pos, int &major, int &minor, quint64 &size) const {
	if (pos >= (quint64)input.size ()) {
		return false;
	}
	major = (quint8)input.at (pos) >> 5;
	minor = (quint8)input.at (pos) & 31;
	if (minor > 27) {
		return false;
	}
	switch (minor) {
	case 24:
		if (pos + 1 >= input.size ()) {
			return false;
		}
		size = (quint64)(quint8)input.at (pos + 1);
		pos += 2;
		break;
	case 25:
		if (pos + 2 >= input.size ()) {
			return false;
		}
		size = (quint64)(quint8)input.at (pos + 1) << 8;
		size |= (quint64)(quint8)input.at (pos + 2);
		pos += 3;
		break;
	case 26:
		if (pos + 4 >= input.size ()) {
			return false;
		}
		size = (quint64)(quint8)input.at (pos + 1) << 24;
		size |= (quint64)(quint8)input.at (pos + 2) << 16;
		size |= (quint64)(quint8)input.at (pos + 3) << 8;
		size |= (quint64)(quint8)input.at (pos + 4);
		pos += 5;
		break;
	case 27:
		if (pos + 8 >= input.size ()) {
			return false;
		}
		size = (quint64)(quint8)input.at (pos + 1) << 56;
		size |= (quint64)(quint8)input.at (pos + 2) << 48;
		size |= (quint64)(quint8)input.at (pos + 3) << 40;
		size |= (quint64)(quint8)input.at (pos + 4) << 32;
		size |= (quint64)(quint8)input.at (pos + 5) << 24;
		size |= (quint64)(quint8)input.at (pos + 6) << 16;
		size |= (quint64)(quint8)input.at (pos + 7) << 8;
		size |= (quint64)(quint8)input.at (pos + 8);
		pos += 9;
		break;
	default:
		size = minor;
		pos += 1;
		break;
	}
	return true;
}
bool Cbor::read (const QByteArray &input, quint64 &pos) {
	int major, minor;
	quint64 pos2 = pos;
	quint64 size;
	while (true) {
		if (!this->readVar (input, pos2, major, minor, size)) {
			return false;
		}
		if (major != 6) {
			break;
		}
		this->m_tags.push_back (size);
	}
	switch (major) {
	case 0:
	case 1:
		this->m_type = (Cbor::Type)major;
		this->m_value = size;
		pos = pos2;
		return true;
	case 2:
		this->m_type = (Cbor::Type)major;
		if (size > (quint64)input.size () && pos2 + size > (quint64)input.size ()) {
			return false;
		}
		this->m_blob = input.mid (pos2, size);
		pos = pos2 + size;
		return true;
	case 3:
		this->m_type = (Cbor::Type)major;
		if (size > (quint64)input.size () && pos + size > (quint64)input.size ()) {
			return false;
		}
		this->m_string = QString::fromUtf8 (input.mid (pos2, size));
		pos = pos2 + size;
		return true;
	case 4:
		this->m_type = (Cbor::Type)major;
		for (quint64 i = 0; i != size; ++i) {
			Cbor child;
			if (!child.read (input, pos2)) {
				return false;
			}
			this->m_list.push_back (child);
		}
		pos = pos2;
		return true;
	case 5:
		this->m_type = (Cbor::Type)major;
		for (quint64 i = 0; i != size; ++i) {
			Cbor key, child;
			if (!key.read (input, pos2)) {
				return false;
			}
			if (!child.read (input, pos2)) {
				return false;
			}
			this->m_map.insert (key, child);
		}
		pos = pos2;
		return true;
	case 7:
		if (minor < 25) {
			this->m_type = SIMPLE;
			this->m_value = size;
			pos = pos2;
			return true;
		} else if (minor == 25) {
			//TODO handle half
			return false;
		} else if (minor == 26) {
			this->m_type = FLOAT;
			union {
				float f;
				quint32 i;
			};
			i = size;
			this->m_float = f;
			pos = pos2;
			return true;
		} else if (minor == 27) {
			this->m_type = FLOAT;
			this->m_value = size;
			pos = pos2;
			return true;
		}
	}
	return false;//Never reached
}
bool Cbor::operator < (const Cbor &other) const {
	//TODO compare tags
	if (this->m_type < other.m_type) {
		return true;
	} else if (this->m_type > other.m_type) {
		return false;
	} else {
		switch (this->m_type) {
		case UNSIGNED:
		case NEGATIVE:
			return this->m_value < other.m_value;
		case TEXT: {
			QByteArray first = this->m_string.toUtf8 ();
			QByteArray second = other.m_string.toUtf8 ();
			if (first.size () < second.size ()) {
				return true;
			} else if (first.size () > second.size ()) {
				return false;
			} else {
				return first < second;
			}
		}
		default:
			//TODO handle other cases
			return false;
		}
	}
}
bool Cbor::operator == (const Cbor &other) const {
	if (this->m_tags != other.m_tags) {
		return false;
	}
	if (this->m_type != other.m_type) {
		return false;
	}
	switch (this->m_type) {
	case BLOB:
		return this->m_blob == other.m_blob;
	case TEXT:
		return this->m_string == other.m_string;
	case ARRAY:
		return this->m_list == other.m_list;
	case MAP:
		return this->m_map == other.m_map;
	case FLOAT:
		return this->m_float == other.m_float;
	default:
		return this->m_value == other.m_value;
	}
}
bool Cbor::operator != (const Cbor &other) const {
	return !(this->operator == (other));
}
QByteArray cborSave (const Cbor &val) {
	return val.save ();
}
Cbor cborLoad (const QByteArray &val) {
	Cbor res;
	if (!res.load (val)) {
		//throw std::runtime_error ("Cbor error");
	}
	return res;
}
QString Cbor::toDebugString () const {
	switch (this->m_type) {
	case UNSIGNED:
		return QString::number (this->m_value);
	case NEGATIVE:
		return "-" + QString::number (this->m_value);
	case BLOB: {
		return "h'" + this->m_blob.toHex ().toLower () + "'";
	}
	case TEXT: {
		QString text;
		text.push_back ('\"');
		foreach (QChar ch, this->m_string) {
			if (ch.unicode () == '\n') {
				text.append ("\\n");
			} else if (ch.unicode () == '\"') {
				text.append ("\\\"");
			} else if (ch.unicode () == '\\') {
				text.append ("\\\\");
			} else {
				text.push_back (ch);
			}
		}
		text.push_back ('\"');
		return text;
	}
	case ARRAY: {
		QString result;
		result += "[";
		bool first = true;
		foreach (Cbor child, this->m_list) {
			if (!first) {
				result += ", ";
			}
			result += child.toDebugString ();
			first = false;
		}
		result += "]";
		return result;
	}
	case MAP: {
		QString result;
		result += "{";
		bool first = true;
		foreach (Cbor key, this->m_map.keys ()) {
			foreach (Cbor value, this->m_map.values (key)) {
				if (!first) {
					result += ", ";
				}
				result += key.toDebugString ();
				result += ": ";
				result += value.toDebugString ();
				first = false;
			}
		}
		result += "}";
		return result;
	}
	case SIMPLE:
		switch (this->m_value) {
		case False:
			return "false";
		case True:
			return "true";
		case Null:
			return "null";
		case Undefined:
			return "undefined";
		default:
			return "simple(" + QString::number (this->m_value) + ")";
		}
	case FLOAT:
		return QString::number (this->m_float);
	default:
		return "?";
	}
}
