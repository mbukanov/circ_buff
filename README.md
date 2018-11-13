# circ_buff

Just circular buffer in C. 

Use example
```C
int i = 0;
char buffer[32] = "";
for(i; i < 15; i++) {
    sprintf(buffer, "string_%d", i);
    circ_open_and_write("./testfile.txt", 200, 10, buffer);
}
```

In testfile.txt:
```
11, string_10
12, string_11
13, string_12
14, string_13
15, string_14
6, string_5
7, string_6
8, string_7
9, string_8
10, string_9
```
