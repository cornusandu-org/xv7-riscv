# xv7

xv7 is a fork of xv6, designed to introduce new features such as more types of locking (1), panic codes (2), or other kinds of features generally found in real-world kernels.

See [the record of changes](./RECORD.md) for a total list of all modifications made to stock xv6.

## Roadmap

| ST | Feature | Progress |
| :-: | :------ | :-------- |
| &#9744; | Signals     | █ |
| &#9744; | `yieldlock` | ███████ |
| &#9744; | More Types  | ████████ |
| &#9745; | Panic Codes | ██████████ |
| &#9745; | Improved Boot Flow | ██████████ |
| &#9745; | Assertions  | ██████████ |
| &#9744; | IRQL System  | ████████ |
| &#9744; | Throwable exceptions | |

<small>(See [TODO](./docs/TODO.md))</small>


# xv6 acknowledgments

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)).  See also https://pdos.csail.mit.edu/6.1810/, which provides
pointers to on-line resources for v6.

The following people have made contributions: Russ Cox (context switching,
locking), Cliff Frey (MP), Xiao Yu (MP), Nickolai Zeldovich, and Austin
Clements.

We are also grateful for the bug reports and patches contributed by
Abhinavpatel00, Takahiro Aoyagi, Marcelo Arroyo, Hirbod Behnam, Silas
Boyd-Wickizer, Anton Burtsev, carlclone, Ian Chen, clivezeng, Dan
Cross, Cody Cutler, Mike CAT, Tej Chajed, Asami Doi,Wenyang Duan,
echtwerner, eyalz800, Nelson Elhage, Saar Ettinger, Alice Ferrazzi,
Nathaniel Filardo, flespark, Peter Froehlich, Yakir Goaron, Shivam
Handa, Matt Harvey, Bryan Henry, jaichenhengjie, Jim Huang, Matúš
Jókay, John Jolly, Alexander Kapshuk, Anders Kaseorg, kehao95,
Wolfgang Keller, Jungwoo Kim, Jonathan Kimmitt, Eddie Kohler, Vadim
Kolontsov, Austin Liew, l0stman, Pavan Maddamsetti, Imbar Marinescu,
Yandong Mao, Matan Shabtay, Hitoshi Mitake, Carmi Merimovich,
mes900903, Mark Morrissey, mtasm, Joel Nider, Hayato Ohhashi,
OptimisticSide, papparapa, phosphagos, Harry Porter, Greg Price, Zheng
qhuo, Quancheng, RayAndrew, Jude Rich, segfault, Ayan Shafqat, Eldar
Sehayek, Yongming Shen, Fumiya Shigemitsu, snoire, Taojie, Cam Tenny,
tyfkda, Warren Toomey, Stephen Tu, Alissa Tung, Rafael Ubal, unicornx,
Amane Uehara, Pablo Ventura, Luc Videau, Xi Wang, WaheedHafez, Keiichi
Watanabe, Lucas Wolf, Nicolas Wolovick, wxdao, Grant Wu, x653, Andy
Zhang, Jindong Zhang, Icenowy Zheng, ZhUyU1997, Zou Chang Wei, and Sandu Bogdan.

# Error Reports

Please inform us about errors or suggestions by using GitHub Issues.

# Building

You will need a RISC-V "newlib" tool chain from
https://github.com/riscv/riscv-gnu-toolchain, and qemu compiled for
riscv64-softmmu.  Once they are installed, and in your shell
search path, you can run "make qemu".

# Copyright
See:
* **General License:** [LICENSE.md](./LICENSE.md)
* **(xv6) MIT License:** [LICENSE.xv6](./LICENSE.xv6)
* **(xv7) xv7 License:** [LICENSE.xv7](./LICENSE.xv7)
* **Copyright Notices:** [NOTICE.md](./NOTICE.md)

<br>
<div align="center">

© Copyright 2026 Sandu Bogdan, licensed under the [**xv7 License**](./LICENSE.xv7)
</div>