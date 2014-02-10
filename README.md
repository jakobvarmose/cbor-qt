# CBOR-Qt

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
