# CBOR-Qt

CBOR-Qt should supports everything in RFC 7049 except for 16-bit floats and indefinite length arrays and maps. Values are typically encoded in the smallest number of bytes. However there are some exceptions, namely: Floating point values are never converted to integers even if this would result in a shorter encoding. Likewise integers are never converted to floating point. Indefinite length encoding is never used. 16-bit floating point values are never used.

### Example

```c++
CborMap map;
map.insert ("foo", "bar");
CborList list;
list.append (-2);
list.append (false);
list.append (Cbor::Simple (100));
map.insert ("baz", list);
QByteArray data = Cbor (map).save ();
qDebug () << data.toHex ();
```
