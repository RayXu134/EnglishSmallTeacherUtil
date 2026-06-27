# English Small Teacher's Utility

---

## Description

A utility for English small teachers to calculate students' points.

### How points are calculated

Each student has a personal point.

1. Sum all group points for each group.
2. Add the total group points to each member's personal point.

---

## Usage

### Build

``` sh
make
```

---

### Example of input.ini

**Notice that the namespaces must be in this order**

1. Group
2. GroupPoint
3. PersonalPoint

``` ini
# Group members
# Syntax: GROUP=MEMBER_A,MEMBER_B,MEMBER_C...
[Group]
1=14,5,10,18
2=13,9,17,8

# Group points (will be summed per group)
# Syntax: GROUP=POINT_A,POINT_B,POINT_C...
[GroupPoint]
1=12,5,9,0
2=7,4,14,8

# Personal student points
# Syntax: STUDENT=POINT
[PersonalPoint]
5=3
8=11
9=2
10=8
13=1
14=7
17=15
18=9
```

---

### Run

There are 3 subcommands, read, gen and help.
- read: Read an ini file and calculate points.
- gen: Generate an ini file template for you.
- help: Print a help message.

#### read

``` sh
./main read ./input.ini
```

#### gen

``` sh
./main gen ./output.ini
```

#### help

``` sh
./main help
```
