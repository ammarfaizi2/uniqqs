# uniqqs

Uniq for URL with query string filter.

# Installation
```
git clone https://github.com/ammarfaizi2/uniqqs
cd uniqqs
gcc -O3 uniqqs.c -o uniqqs
uniqqs < test.txt
```

# Example Case
### List
```
https://www.facebook.com/endpoint_a?p1=123&p2=123123
https://www.facebook.com/endpoint_a?p4=123&p3=123123
https://www.facebook.com/endpoint_b?p4=123&p3=123123
https://www.facebook.com/endpoint_b?p5=123&p6=123123
https://www.facebook.com/endpoint_a?p5=123&p6=123123
https://www.facebook.com/endpoint_b?p1=123&p2=123123
```
### Result
```
https://www.facebook.com/endpoint_a?p1=xxx&p2=xxx&p4=xxx&p3=xxx&p5=xxx&p6=xxx
https://www.facebook.com/endpoint_b?p4=xxx&p3=xxx&p5=xxx&p6=xxx&p1=xxx&p2=xxx
```

# License
MIT
