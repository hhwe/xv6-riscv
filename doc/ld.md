# [链接器脚本](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC5)

> 原文: https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC17

每个链接都由**链接器脚本**控制。这个脚本是 用链接器命令语言编写。

链接器脚本的主要用途是描述各节如何 输入文件应映射到输出文件中，并控制 输出文件的内存布局。大多数链接器脚本不执行任何操作 不止于此。但是，在必要时，链接器脚本还可以 使用以下命令指示链接器执行许多其他操作 如下所述。

链接器始终使用链接器脚本。如果您不提供 你自己，链接器将使用一个默认脚本，该脚本被编译到 链接器可执行文件。您可以使用命令行选项 以显示默认链接器脚本。某些命令行选项， 例如 或 ，将影响默认链接器脚本。\`--verbose'\`-r'\`-N'

您可以使用以下命令提供自己的链接器脚本 line 选项。执行此操作时，链接器脚本将替换 默认链接器脚本。\`-T'

您还可以通过将链接器脚本命名为输入文件来隐式使用链接器脚本 到链接器，就好像它们是要链接的文件一样。请参阅[隐式链接器脚本](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC48)部分。

- [基本脚本概念](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC6)：基本链接器脚本概念
- [脚本格式](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC7)：链接器脚本格式
- [简单示例](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC8)：简单链接器脚本示例
- [简单命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC9)：简单链接器脚本命令
- [赋值](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC14)：为符号赋值
- [SECTIONS](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC17)：SECTIONS 命令
- [MEMORY：](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC37)MEMORY 命令
- [PHDRS](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC38)：PHDRS 命令
- [VERSION：](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC39)VERSION 命令
- [表达式](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC40)：链接器脚本中的表达式
- [隐式链接器脚本](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC48)：隐式链接器脚本

## [链接器脚本基本概念](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC6)

我们需要定义一些基本概念和词汇，以便 描述链接器脚本语言。

链接器将输入文件合并到单个输出文件中。输出 文件和每个输入文件都采用一种称为**对象文件格式**的特殊数据格式。每个文件都称为一个**目标文件**。 输出文件通常称为**可执行文件**，但对于我们的 目的 我们也将它称为目标文件。每个目标文件都有， 除其他事项外，**还有部分**列表。我们有时指的是 部分作为输入文件中的**输入部分**;同样，一个部分 输出文件中是一个**输出部分**。

对象文件中的每个部分都有一个名称和大小。大多数部分 还具有关联的数据块，称为 **内容**。一个部分可以标记为**可加载**，这意味着 运行输出文件时，应将内容加载到内存中。 没有内容的部分可能是**可分配**的，这意味着 内存中的区域应该被留出来，但没有什么特别的不应该被留出来 加载到那里（在某些情况下，此内存必须清零）。A 部分 既不可加载也不可分配的通常包含某种排序 的调试信息。

每个可加载或可分配的输出部分都有两个地址。这 首先是 **VMA** 或虚拟内存地址。这是地址 该部分将在运行输出文件时具有。第二个是 **LMA**，即加载内存地址。这是 部分将被加载。在大多数情况下，这两个地址将是 相同。它们可能不同的一个例子是当数据部分 加载到ROM中，然后在程序启动时复制到RAM中 （此技术通常用于初始化 ROM 中的全局变量 基于系统）。在这种情况下，ROM 地址将是 LMA，而 RAM 地址将是 VMA。

您可以使用带有选项的程序来查看对象文件中的各个部分。`objdump`\`-h'

每个对象文件还具有一个**符号**列表，称为**符号表**。符号可以是定义的，也可以是未定义的。每个符号 具有名称，每个定义的符号都具有地址等 信息。如果将 C 或 C++ 程序编译到目标文件中，则 将为每个定义的函数和全局或 静态变量。每个未定义的函数或全局变量 输入文件中引用的符号将变为未定义的符号。

您可以使用该程序或使用带有该选项的程序来查看对象文件中的符号。`nm``objdump`\`-t'

## [链接器脚本格式](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC7)

链接器脚本是文本文件。

将链接器脚本编写为一系列命令。每个命令都是 要么是关键字，后面可能跟着参数，要么是赋值给 象征。您可以使用分号分隔命令。空格是 通常被忽略。

通常可以直接输入文件或格式名称等字符串。 如果文件名包含逗号等字符，则 否则用于分隔文件名，您可以将文件名放在 双引号。无法在 文件名。

您可以像在 C 语言中一样在链接器脚本中包含注释，以 和 分隔。与 C 语言一样，注释在语法上是等价的 到空格。\`/\*'\`\*/'

## [简单链接器脚本示例](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC8)

许多链接器脚本都相当简单。

最简单的链接器脚本只有一个命令：.您可以使用该命令来描述 输出文件的内存布局。\`SECTIONS'\`SECTIONS'

该命令是一个强大的命令。在这里，我们将 描述它的简单用法。假设您的程序仅包含以下内容 代码、初始化数据和未初始化数据。它们将分别位于 、 和 部分中。 让我们进一步假设这些是出现在 您的输入文件。\`SECTIONS'\`.text'\`.data'\`.bss'

在此示例中，假设代码应在地址加载 0x10000，并且数据应从地址 0x8000000 开始。这是一个 链接器脚本将执行此操作：

```
SECTIONS
{
  . = 0x10000;
  .text : { *(.text) }
  . = 0x8000000;
  .data : { *(.data) }
  .bss : { *(.bss) }
}
```

你把命令写成关键字， 后面是一系列符号分配和输出部分 用大括号括起来的描述。\`SECTIONS'\`SECTIONS'

上面示例中的第一行设置特殊符号， 这是位置计数器。如果未指定 输出部分（其他方式将在后面描述），the address 是根据 location 计数器的当前值设置的。这 然后，位置计数器按输出部分的大小递增。\`.'

上述示例的命令内的第一行 设置特殊符号的值，即位置 计数器。如果未指定某些输出部分的地址 其他方式（其他方式将在后面描述），地址是从 位置计数器的当前值。然后位置计数器是 按输出部分的大小递增。在命令的开头，位置计数器的值为 。\`SECTIONS'\`.'\`SECTIONS'\`0'

第二行定义输出部分 。冒号是 所需的语法，暂时可以忽略。在大括号内 在输出节名称之后，列出输入节的名称 它应该放在这个输出部分。这是一个 与任何文件名匹配的通配符。表达式表示所有输入文件中的所有输入部分。\`.text'\`\*'\`\*(.text)'\`.text'

由于位置计数器是在定义输出部分时，因此链接器会将输出文件中部分的地址设置为 。\`0x10000'\`.text'\`.text'\`0x10000'

其余行定义 和 部分 输出文件。链接器将放置输出部分 在地址。链接器放置输出部分后，位置计数器的值将加输出部分的大小。这 效果是链接器将放置输出部分 紧接在内存中的输出部分之后\`.data'\`.bss'\`.data'\`0x8000000'\`.data'\`0x8000000'\`.data'\`.bss'\`.data'

链接器将确保每个输出部分都具有所需的 对齐，如有必要，通过增加位置计数器。在这个 例如，为 和 部分指定的地址可能满足任何对齐约束，但链接器 可能必须在 和 部分之间留出一个小间隙。\`.text'\`.data'\`.data'\`.bss'

就是这样！这是一个简单而完整的链接器脚本。

## [简单链接器脚本命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC9)

在本节中，我们将介绍简单的链接器脚本命令。

- [入口点](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC10)：设置入口点
- [文件命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC11)：处理文件的命令
- [格式化命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC12)：处理对象文件格式的命令
- [其他命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC13)：其他链接器脚本命令

### [设置入口点](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC10)

在程序中执行的第一条指令称为**条目 点**。您可以使用 linker script 命令将 入口点。参数是一个符号名称：`ENTRY`

```
ENTRY(<var>symbol</var>)
```

有几种方法可以设置入口点。链接器将设置 入口点，按顺序尝试以下每种方法，以及 当其中一个成功时停止：

- 命令行选项;\`-e'entry
- 链接器脚本中的命令;`ENTRY(symbol)`
- 符号的值 ，如果已定义;`start`
- 该部分的第一个字节的地址（如果存在）;\`.text'
- 地址 .`0`

### [处理文件的命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC11)

多个链接器脚本命令处理文件。

`INCLUDE filename`

此时包括链接器脚本。该文件将 在当前目录和任何指定的目录中搜索 替换为选项。您最多可以嵌套调用 10级深。filename`-L``INCLUDE`

`INPUT(file, file, ...)`

`INPUT(file file ...)`

该命令指示链接器包含命名文件 在链接中，就好像它们是在命令行上命名的一样。 例如，如果您总是希望在任何时候都包含“subr.o” 一个链接，但你不能费心把它放在每个链接命令行上， 然后，您可以放入链接器脚本。 事实上，如果您愿意，可以在链接器中列出所有输入文件 脚本，然后调用链接器，只使用一个选项。 链接器将首先尝试打开当前目录中的文件。如果 未找到，链接器将搜索存档库 搜索路径。请参阅[命令行选项](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_2.html#SEC3)部分中的说明。 如果使用 ，则会将 name 设置为 ，与命令行参数一样。 在隐式链接器脚本中使用该命令时， 文件将包含在链接器所在的链接中 包括脚本文件。这可能会影响存档搜索。`INPUT`\`INPUT (subr.o)'\`-T'\`-L'\`INPUT (-lfile)'`ld``libfile.a`\`-l'`INPUT`

`GROUP(file, file, ...)`

`GROUP(file file ...)`

该命令类似于 ，只是命名的 文件都应该是存档，并且会反复搜索，直到没有 将创建新的未定义引用。请参阅[命令行选项](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_2.html#SEC3)部分中的说明。`GROUP``INPUT`\`-('

`OUTPUT(filename)`

该命令为输出文件命名。在链接器脚本中使用与在命令行中使用完全相同（请参阅[命令行选项](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_2.html#SEC3)部分）。如果两者都使用，则命令行选项将 优先。 您可以使用该命令定义 输出文件，而不是通常的默认值“a.out”。`OUTPUT``OUTPUT(filename)`\`-o filename'`OUTPUT`

`SEARCH_DIR(path)`

该命令将添加到查找存档库的路径列表中。使用与在命令行上使用完全相同（请参阅[命令行选项](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_2.html#SEC3)部分）。如果两者兼而有之 ，则链接器将搜索这两个路径。使用 首先搜索命令行选项。`SEARCH_DIR`path`ld``SEARCH_DIR(path)`\`-L path'

`STARTUP(filename)`

该命令与命令一样，只是 这将成为第一个要链接的输入文件，因为 尽管它首先在命令行上指定。这可能很有用 当使用入口点始终是 第一个文件。`STARTUP``INPUT`filename

### [处理对象文件格式的命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC12)

有几个链接器脚本命令处理对象文件格式。

`OUTPUT_FORMAT(bfdname)`

`OUTPUT_FORMAT(default, big, little)`

该命令命名要用于 输出文件（参见 [BFD](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_5.html#SEC53) 部分）。使用 是 就像在命令行上使用 （请参阅[命令行选项](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_2.html#SEC3)部分）。如果两者都使用，则命令 line 选项优先。 您可以与三个参数一起使用，以使用不同的 基于 AND 命令行选项的格式。 这允许链接器脚本根据 所需的字节序。 如果既不使用 也不使用 ，则输出格式 将是第一个参数，.如果使用，则 输出格式将是第二个参数 。如果是 使用时，输出格式将是第三个参数 。 例如，MIPS ELF 目标的默认链接器脚本使用此 command：这表示输出文件的默认格式是 ，但如果用户使用命令行 选项，输出文件将以 Format 创建。`OUTPUT_FORMAT``OUTPUT_FORMAT(bfdname)`\`-oformat bfdname'`OUTPUT_FORMAT`\`-EB'\`-EL'\`-EB'\`-EL'default\`-EB'big\`-EL'little

```
OUTPUT_FORMAT(elf32-bigmips, elf32-bigmips, elf32-littlemips)
```

\`elf32-bigmips'\`-EL'\`elf32-littlemips'

`TARGET(bfdname)`

该命令命名读取输入时要使用的 BFD 格式 文件。它会影响后续和命令。 此命令类似于在命令行上使用 （请参阅[命令行选项](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_2.html#SEC3)部分）。如果命令 ，则最后一个命令也用于设置输出文件的格式。请参阅 [BFD](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_5.html#SEC53) 部分。`TARGET``INPUT``GROUP`\`-b bfdname'`TARGET``OUTPUT_FORMAT``TARGET`

### [其他链接器脚本命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC13)

还有一些其他链接器脚本命令。

`ASSERT(exp, message)`

确保它不为零。如果为零，则退出链接器 替换为错误代码，然后打印 .expmessage

`EXTERN(symbol symbol ...)`

在输出文件中输入的力为未定义 象征。例如，这样做可能会触发其他 标准库中的模块。您可以列出几个 每个，您可以多次使用。这 命令与命令行选项具有相同的效果。symbolsymbol`EXTERN``EXTERN`\`-u'

`FORCE_COMMON_ALLOCATION`

此命令与命令行选项具有相同的效果： 为公共符号分配空间，即使可重定位 输出文件指定为 （）。\`-d'`ld`\`-r'

`NOCROSSREFS(section section ...)`

此命令可用于告诉发出有关任何 某些输出部分之间的引用。 在某些类型的程序中，特别是在嵌入式系统上 使用叠加，当一个部分加载到内存中时，另一个部分 不会。这两个部分之间的任何直接引用都是 错误。例如，如果一个部分中的代码调用 在另一节中定义的函数。 该命令获取输出节名称的列表。如果检测到各部分之间的任何交叉引用，它会报告 错误并返回非零退出状态。请注意，该命令使用输出节名称，而不是输入节 名字。`ld``NOCROSSREFS``ld``NOCROSSREFS`

`OUTPUT_ARCH(bfdarch)`

指定特定的输出计算机体系结构。论点是一个 BFD 库使用的名称（参见 [BFD](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_5.html#SEC53) 部分）。您可以看到 通过将程序与 选项。`objdump`\`-f'

## [为符号赋值](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC14)

您可以为链接器脚本中的符号赋值。这将定义 作为全局符号的符号。

- [简单作业](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC15)：简单作业
- [提供](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC16)：提供

### [简单作业](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC15)

您可以使用任何 C 赋值运算符赋值符号：

`symbol = expression ;`

`symbol += expression ;`

`symbol -= expression ;`

`symbol *= expression ;`

`symbol /= expression ;`

`symbol <<= expression ;`

`symbol >>= expression ;`

`symbol &= expression ;`

`symbol |= expression ;`

第一种情况将定义为 的值。在其他情况下，必须已经是 定义，值将相应调整。symbolexpressionsymbol

特殊符号名称表示位置计数器。你 只能在命令中使用它。\`.'`SECTIONS`

后面的分号是必需的。expression

表达式定义如下;请参阅[链接器脚本中的表达式](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC40)部分。

您可以将符号赋值单独编写为命令，也可以将符号分配编写为 命令中的语句，或作为输出的一部分 命令中的节描述。`SECTIONS``SECTIONS`

符号的部分将从 表达;有关详细信息，请参阅[表达式部分](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC46)。

下面是一个示例，显示了符号的三个不同位置 可以使用以下作业：

```
floating_point = 0;
SECTIONS
{
  .text :
    {
      *(.text)
      _etext = .;
    }
  _bdata = (. + 3) &amp; ~ 4;
  .data : { *(.data) }
}
```

在此示例中，符号将定义为 零。该符号将定义为以下地址 最后一个输入部分。符号将是 定义为输出部分对齐后的地址 向上到 4 字节边界。\`floating\_point'\`\_etext'\`.text'\`\_bdata'\`.text'

### [提供](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC16)

在某些情况下，链接器脚本需要定义符号 仅当它被引用并且未被包含在 链接。例如，传统的链接器定义了符号 。但是，ANSI C 要求用户能够用作函数名称而不会遇到错误。关键字可用于定义符号，例如 ，仅当它被引用但未定义时。语法为 。\`etext'\`etext'`PROVIDE`\`etext'`PROVIDE(symbol = expression)`

下面是用于定义的示例：`PROVIDE`\`etext'

```
SECTIONS
{
  .text :
    {
      *(.text)
      _etext = .;
      PROVIDE(etext = .);
    }
}
```

在此示例中，如果程序定义 （使用前导 下划线），链接器将给出多重定义错误。如果，开 另一方面，程序定义（没有前导 下划线），链接器将在程序中以静默方式使用定义。 如果程序引用但未定义它，则 链接器将使用链接器脚本中的定义。\`\_etext'\`etext'\`etext'

## [SECTIONS 命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC17)

该命令告诉链接器如何映射输入部分 到输出部分，以及如何将输出部分放置在内存中。`SECTIONS`

该命令的格式为：`SECTIONS`

```
SECTIONS
{
  <var>sections-command</var>
  <var>sections-command</var>
  ...
}
```

每个都可以是以下其中一项：sections-command

- 命令（请参阅[设置入口点](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC10)`ENTRY`)
- 符号分配（请参阅[为符号赋值](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC14)一节)
- 输出部分说明
- 叠加层说明

命令和符号分配允许在命令内部进行，以便于在 这些命令。这也可以使链接器脚本更容易 理解，因为您可以在 输出文件的布局。`ENTRY``SECTIONS`

描述了输出部分描述和叠加描述 下面。

如果未在链接器脚本中使用命令，则 链接器会将每个输入部分放入同名的输出中 部分，按照这些部分在 输入文件。如果第一个文件中存在所有输入部分，则 例如，输出文件中各节的顺序将与顺序匹配 在第一个输入文件中。第一部分将位于地址零处。`SECTIONS`

- [输出部分说明](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC18)：输出部分说明
- [输出节名称](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC19)：输出节名称
- [输出段地址](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC20)：输出段地址
- [输入部分](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC21)：输入部分说明
- [输出截面数据](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC27)：输出截面数据
- [输出部分关键字](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC28)： 输出部分关键字
- [输出段丢弃](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC29)：输出段丢弃
- [输出节属性](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC30)：输出节属性
- [叠加描述](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC36)：叠加描述

### [输出部分说明](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC18)

输出部分的完整描述如下所示：

```
<var>section</var> [<var>address</var>] [(<var>type</var>)] : [AT(<var>lma</var>)]
  {
    <var>output-section-command</var>
    <var>output-section-command</var>
    ...
  } [&gt;<var>region</var>] [:<var>phdr</var> :<var>phdr</var> ...] [=<var>fillexp</var>]
```

大多数输出节不使用大多数可选节属性。

周围的空格是必需的，因此该部分 名称是明确的。还需要冒号和大括号。 换行符和其他空格是可选的。section

每个都可以是以下项之一：output-section-command

- 符号分配（请参阅[为符号赋值](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC14)一节)
- 输入部分说明（请参阅[输入部分说明](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC21)部分)
- 要直接包含的数据值（请参阅[输出部分数据](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC27)部分)
- 特殊的输出部分关键字（请参阅[输出部分关键字](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC28)部分)

### [输出节名称](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC19)

输出部分的名称为 。 必须 满足输出格式的约束。在仅 支持有限数量的部分，例如 、名称 必须是格式 （， for 例如，仅允许 或 ）。如果 输出格式支持任意数量的部分，但支持数字而不是 名称（如 Oasy 的情况），名称应以 带引号的数字字符串。节名可以由以下任何序列组成 字符，但名称包含任何不寻常的字符，例如 逗号必须加引号。sectionsection`a.out``a.out`\`.text'\`.data'\`.bss'

输出部分名称是特殊的;部分 [输出部分丢弃](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC29)。\`/DISCARD/'

### [输出段地址](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC20)

是 VMA（虚拟内存）的表达式 address） 的输出部分。如果您不提供 ， 链接器将根据是否存在或其他情况进行设置 基于位置计数器的当前值。addressaddressregion

如果提供 ，则输出部分的地址将为 正是这样设置的。如果既不提供 也不提供 ，则输出部分的地址将设置为 与对齐方式对齐的位置计数器的当前值 输出部分的要求。的对齐要求 输出部分是包含的任何输入部分的最严格对齐方式 在输出部分中。addressaddressregion

例如

```
.text . : { *(.text) }
```

和

```
.text : { *(.text) }
```

略有不同。第一个会将输出部分的地址设置为该位置的当前值 计数器。第二个会将其设置为位置的当前值 计数器与输入的最严格对齐方式对齐 部分。\`.text'\`.text'

可以是任意表达式;[部分链接器脚本中的表达式](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC40)。 例如，如果要在 0x10 字节边界上对齐该部分， 因此，节地址的最低四位为零，您可以 做这样的事情：address

```
.text ALIGN(0x10) : { *(.text) }
```

这之所以有效，是因为返回当前位置计数器 向上对齐到指定值。`ALIGN`

指定节将更改 位置计数器。address

### [输入部分说明](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC21)

最常见的输出节命令是输入节描述。

输入部分描述是最基本的链接器脚本操作。 您可以使用输出部分告诉链接器如何布局程序 在内存中。您可以使用输入部分说明来告诉链接器如何 将输入文件映射到内存布局中。

- [输入部分基础](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC22)知识：输入部分基础知识
- [输入部分通配符](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC23)：输入部分通配符模式
- [Input Section Common](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC24)：公共符号的输入部分
- [输入部分保留](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC25)：输入部分和垃圾回收
- [输入部分示例](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC26)：输入部分示例

#### [输入部分基础知识](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC22)

输入部分描述由文件名（可选）组成 括号中的章节名称列表。

文件名和节名可以是通配符模式，我们 在下面进一步描述（请参阅[输入部分通配符模式](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC23)部分）。

最常见的输入部分描述是包括所有输入 在输出部分中具有特定名称的部分。例如，要 包括所有输入部分，您将编写：\`.text'

```
*(.text)
```

这里是一个与任何文件名匹配的通配符。\`\*'

有两种方法可以包含多个部分：

```
*(.text .rdata)
*(.text) *(.rdata)
```

它们之间的区别在于 和 输入部分在输出部分中出现的顺序。在 第一个例子，它们将混合在一起。在第二个示例中，所有输入部分将首先显示，然后是所有输入部分。\`.text'\`.rdata'\`.text'\`.rdata'

您可以指定文件名以包含特定文件中的各节。 如果一个或多个文件包含以下特殊数据，则可以执行此操作 需要位于内存中的特定位置。例如：

```
data.o(.data)
```

如果使用的文件名没有部分列表，则所有部分 输入文件将包含在输出部分中。这不是 通常这样做，但有时可能有用。例如：

```
data.o
```

当您使用不包含任何通配符的文件名时 字符，链接器将首先查看您是否也指定了文件 链接器命令行或命令中的名称。如果你 没有，链接器将尝试将文件作为输入文件打开，因为 虽然它出现在命令行上。请注意，这与命令不同，因为链接器不会在 存档搜索路径。`INPUT``INPUT`

#### [输入部分通配符模式](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC23)

在输入节描述中，文件名或节 名称或两者可以是通配符模式。

在许多示例中看到的文件名是一个简单的通配符 模式作为文件名。\`\*'

通配符模式类似于 Unix shell 使用的通配符模式。

\`\*'

匹配任意数量的字符

\`?'

匹配任何单个字符

\`\[chars\]'

匹配任何 ;该字符可用于指定字符范围，如匹配任何小写字母chars\`-'\`\[a-z\]'

\`\\'

引用以下字符

当文件名与通配符匹配时，通配符 将不匹配字符（用于分隔目录名称 Unix）。由单个字符组成的模式是 例外;它将始终匹配任何文件名，无论它是否包含 a。在节名称中，通配符将匹配 一个角色。\`/'\`\*'\`/'\`/'

文件名通配符模式仅匹配显式 在命令行或命令中指定。链接器 不搜索目录以展开通配符。`INPUT`

如果文件名与多个通配符模式匹配，或者如果文件名 显式显示，并且还与通配符模式（链接器）匹配 将使用链接器脚本中的第一个匹配项。例如，这个 输入部分描述的顺序可能出错，因为不会使用“data.o”规则：

```
.data : { *(.data) }
.data1 : { data.o(.data) }
```

通常，链接器将放置与通配符匹配的文件和部分 按照它们在链接期间的显示顺序。您可以更改 这是通过使用关键字，该关键字出现在通配符之前 括号中的模式（例如，）。使用关键字时，链接器将对文件或部分进行排序 在将它们放入输出文件之前，按名称升序排列。`SORT``SORT(.text*)``SORT`

如果您对输入部分的去向感到困惑，请使用链接器选项生成地图文件。地图文件显示 精确地将输入部分映射到输出部分。\`-M'

此示例演示如何使用通配符模式进行分区 文件。此链接器脚本指示链接器将所有部分放在 中，并将所有部分放在 中。 链接器将放置所有文件开头的部分 中带有大写字符;对于所有其他文件， 链接器会将该部分放在 中。\`.text'\`.text'\`.bss'\`.bss'\`.data'\`.DATA'\`.data'\`.data'

```
SECTIONS {
  .text : { *(.text) }
  .DATA : { [A-Z]*(.data) }
  .data : { *(.data) }
  .bss : { *(.bss) }
}
```

#### [常用符号的输入部分](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC24)

常用符号需要特殊的符号，因为在许多对象中 文件格式 常用符号没有特定的输入部分。这 链接器将常用符号视为输入部分中的符号 叫。\`COMMON'

您可以像使用任何文件名一样在该部分中使用文件名 其他输入部分。您可以使用它来放置 一个部分中的特定输入文件，而其他部分的常见符号 输入文件放置在另一个部分。\`COMMON'

在大多数情况下，输入文件中的常用符号将放置在输出文件的部分中。例如：\`.bss'

```
.bss { *(.bss) *(COMMON) }
```

某些对象文件格式具有多种类型的通用符号。为 例如，MIPS ELF 对象文件格式区分标准通用 符号和小的常用符号。在这种情况下，链接器将使用 其他类型的常用符号的不同特殊部分名称。在 MIPS ELF的情况，链接器用于标准通用 符号和小的通用符号。这允许您 将不同类型的常用符号映射到不同位置的内存中 地点。\`COMMON'\`.scommon'

您有时会在旧的链接器脚本中看到。这 符号现在被认为是过时的。它等价于 。\`\[COMMON\]'\`\*(COMMON)'

#### [输入部分和垃圾回收](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC25)

使用链路时垃圾回收 （） 时， 标记不应删除的部分通常很有用。 这是通过围绕输入部分的通配符条目来实现的 与 、 如 或 。\`--gc-sections'`KEEP()``KEEP(*(.init))``KEEP(SORT(*)(.ctors))`

#### [输入部分示例](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC26)

以下示例是一个完整的链接器脚本。它告诉链接器 从文件“all.o”中读取所有部分，并将它们放在 从位置开始的输出部分的开始。文件“foo.o”中的所有部分都紧随其后，在同一个输出部分中。“foo.o”中的所有部分都进入输出部分，然后是“foo1.o”中的部分。 所有剩余部分和任何部分 文件被写入输出部分。\`outputa'\`0x10000'\`.input1'\`.input2'\`outputb'\`.input1'\`.input1'\`.input2'\`outputc'

```
SECTIONS {
  outputa 0x10000 :
    {
    all.o
    foo.o (.input1)
    }
  outputb :
    {
    foo.o (.input2)
    foo1.o (.input1)
    }
  outputc :
    {
    *(.input1)
    *(.input2)
    }
}
```

### [输出截面数据](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC27)

您可以使用 、 、 、 或 作为 在输出节中包含显式数据字节 输出部分命令。每个关键字后面都跟着一个表达式 提供要存储的值的括号（请参阅[链接器脚本中的表达式](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC40)部分）。这 表达式的值存储在位置的当前值处 计数器。`BYTE``SHORT``LONG``QUAD``SQUAD`

、 、 和 命令 分别存储 1、2、4 和 8 个字节。存储 字节，则位置计数器按字节数递增 数据处理。`BYTE``SHORT``LONG``QUAD`

例如，这将存储字节 1，后跟 4 字节值 的符号：\`addr'

```
BYTE(1)
LONG(addr)
```

使用 64 位主机或目标时，并且 相同;它们都存储一个 8 字节或 64 位的值。当主机和 目标为 32 位，表达式计算为 32 位。在这种情况下，存储扩展到 64 位的 32 位值零，并存储扩展到 64 位的 32 位值符号。`QUAD``SQUAD``QUAD``SQUAD`

如果输出文件的目标文件格式具有显式字节序， 这是正常情况，该值将存储在该字节序中。 当目标文件格式没有显式字节序时，如 例如，S 记录，则该值将存储在 第一个输入对象文件的字节序。

您可以使用该命令设置填充模式 当前部分。后面是括号中的表达式。任何 该部分中未指定的内存区域（例如， 由于输入部分需要对齐而留下的间隙）被填补 使用表达式的两个最低有效字节，重复为 必要。语句涵盖 它在部分定义中出现的点;通过包含更多 与一个语句相比，您可以在 输出部分的不同部分。`FILL``FILL``FILL`

此示例演示如何使用 价值：\`0x9090'

```
FILL(0x9090)
```

该命令类似于输出 section 属性（参见 [Output section fill](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC35) 部分），但它只影响 命令后面的部分的一部分，而不是 整个部分。如果两者都使用，则命令需要 优先。`FILL`\`=fillexp'`FILL``FILL`

### [输出部分关键字](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC28)

有几个关键字可以显示为输出部分 命令。

`CREATE_OBJECT_SYMBOLS`

该命令告诉链接器为每个输入文件创建一个符号。 每个交易品种的名称将是相应输入的名称 文件。每个交易品种的部分将是输出部分，其中 此时将显示该命令。 这是 a.out 对象文件格式的常规格式。事实并非如此 通常用于任何其他对象文件格式。`CREATE_OBJECT_SYMBOLS`

`CONSTRUCTORS`

使用 a.out 对象文件格式进行链接时，链接器使用 不寻常的集合构造，以支持 C++ 全局构造函数和 析 构 函数。链接不支持的目标文件格式时 任意部分，例如 ECOFF 和 XCOFF，链接器将 按名称自动识别 C++ 全局构造函数和析构函数。 对于这些对象文件格式，该命令告诉 链接器，用于将构造函数信息放置在显示命令的输出部分中。该命令是 对于其他对象文件格式，则忽略。 该符号标志着全球的开始 构造函数，符号标记结束。这 列表中的第一个词是条目数，然后是地址 每个构造函数或析构函数，后跟一个零字。这 编译器必须安排实际运行代码。对于这些目标文件 格式 GNU C++ 通常从子例程调用构造函数;对 的调用将自动插入到 的启动代码。GNU C++ 正常运行 析构函数要么使用 ，要么直接从函数 . 对于支持 或 等对象文件格式 任意部分名称，GNU C++ 通常会安排把 将全局构造函数和析构函数的地址放入 AND 部分。将以下序列放入 链接器脚本将构建 GNU C++ 运行时代码期望看到。 如果您使用 GNU C++ 支持初始化优先级， 它提供了对全局构造函数顺序的一些控制 运行时，您必须在链接时对构造函数进行排序，以确保它们 以正确的顺序执行。使用该命令时，请改用。使用 和 节时，请使用 and 而不是 just 和 。 通常，编译器和链接器会自动处理这些问题， 而且您无需担心它们。但是，您可以 如果您使用 C++ 并编写自己的链接器，则需要考虑这一点 脚本。`CONSTRUCTORS``CONSTRUCTORS``CONSTRUCTORS``__CTOR_LIST__``__DTOR_LIST``__main``__main``main``atexit``exit``COFF``ELF``.ctors``.dtors`

```
      __CTOR_LIST__ = .;
      LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
      *(.ctors)
      LONG(0)
      __CTOR_END__ = .;
      __DTOR_LIST__ = .;
      LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)
      *(.dtors)
      LONG(0)
      __DTOR_END__ = .;
```

`CONSTRUCTORS`\`SORT(CONSTRUCTORS)'`.ctors``.dtors`\`\*(SORT(.ctors))'\`\*(SORT(.dtors))'\`\*(.ctors)'\`\*(.dtors)'

### [输出部分丢弃](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC29)

链接器不会创建没有任何输出部分 内容。这是为了方便在引用以下输入部分时 可能存在于任何输入文件中，也可能不存在于任何输入文件中。例如：

```
.foo { *(.foo) }
```

仅当至少一个输入文件中存在一个部分时，才会在输出文件中创建一个部分。\`.foo'\`.foo'

如果使用输入部分描述以外的任何内容作为输出 section 命令，例如符号赋值，然后输出部分 将始终被创建，即使没有匹配的输入部分也是如此。

特殊输出部分名称可用于丢弃 输入部分。分配给输出的任何输入部分 部分未包含在输出文件中。\`/DISCARD/'\`/DISCARD/'

### [输出部分属性](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC30)

我们在上面展示了输出部分的完整描述 喜欢这个：

```
<var>section</var> [<var>address</var>] [(<var>type</var>)] : [AT(<var>lma</var>)]
  {
    <var>output-section-command</var>
    <var>output-section-command</var>
    ...
  } [&gt;<var>region</var>] [:<var>phdr</var> :<var>phdr</var> ...] [=<var>fillexp</var>]
```

我们已经描述了 、 和 。在本节中，我们将介绍 其余部分属性。sectionaddressoutput-section-command

- [输出段类型](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC31)：输出段类型
- [输出段 LMA](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC32)：输出段 LMA
- [输出部分区域](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC33)：输出部分区域
- [输出部分 Phdr](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC34)：输出部分 phdr
- [输出部分填充](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC35)：输出部分填充

#### [输出段类型](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC31)

每个输出部分可以有一个类型。类型是 括弧。定义了以下类型：

`NOLOAD`

该部分应标记为不可加载，以便它不会 程序运行时加载到内存中。

`DSECT`

`COPY`

`INFO`

`OVERLAY`

支持这些类型名称以实现向后兼容性，并且 很少使用。它们都具有相同的效果：该部分应该是 标记为不可分配，因此不会为 部分。

链接器通常根据 映射到其中的输入部分。您可以使用以下方法覆盖此项 截面类型。例如，在下面的脚本示例中，该部分在内存位置寻址，但不 需要在程序运行时加载。该部分的内容将像往常一样显示在链接器输出文件中。\`ROM'\`0'\`ROM'

```
SECTIONS {
  ROM 0 (NOLOAD) : { ... }
  ...
}
```

#### [输出部分 LMA](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC32)

每个部分都有一个虚拟地址 （VMA） 和一个加载地址 （LMA）;看 [基本链接器脚本概念](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC6)部分。可能出现在 输出部分描述设置 VMA（请参阅[输出部分地址](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC20)部分）。

链接器通常会将 LMA 设置为等于 VMA。您可以更改 通过使用关键字。表达式 在关键字后面指定截面的加载地址。`AT`lma`AT`

此功能旨在使构建 ROM 映像变得容易。为 例如，以下链接器脚本创建三个输出部分：一个 调用的 ，从 开始，一个调用，在节的末尾加载 即使它的 VMA 是 ，并且一个被调用来持有 地址 中未初始化的数据。符号是 用值定义，表示位置 counter 保存 VMA 值，而不是 LMA 值。\`.text'`0x1000`\`.mdata'\`.text'`0x2000`\`.bss'`0x3000``_data``0x2000`

```
SECTIONS
  {
  .text 0x1000 : { *(.text) _etext = . ; }
  .mdata 0x2000 : 
    AT ( ADDR (.text) + SIZEOF (.text) )
    { _data = . ; *(.data); _edata = . ;  }
  .bss 0x3000 :
    { _bstart = . ;  *(.bss) *(COMMON) ; _bend = . ;}
}
```

用于生成的程序的运行时初始化代码 此链接器脚本将包含如下内容，以复制 从 ROM 映像到其运行时地址的初始化数据。通知 此代码如何利用链接器定义的符号 脚本。

```
extern char _etext, _data, _edata, _bstart, _bend;
char *src = &amp;_etext;
char *dst = &amp;_data;

/* ROM has data at end of text; copy it. */
while (dst &lt; &amp;_edata) {
  *dst++ = *src++;
}

/* Zero bss */
for (dst = &amp;_bstart; dst&lt; &amp;_bend; dst++)
  *dst = 0;
```

#### [输出部分区域](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC33)

您可以通过以下方式将部分分配给先前定义的内存区域 用。请参阅 [MEMORY 命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC37)部分。\`>region'

下面是一个简单的示例：

```
MEMORY { rom : ORIGIN = 0x1000, LENGTH = 0x1000 }
SECTIONS { ROM : { *(.text) } &gt;rom }
```

#### [输出部分 phdr](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC34)

您可以通过以下方式将一个部分分配给先前定义的程序段 用。请参阅 [PHDRS 命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC38)部分。如果将某个部分分配给 一个或多个段，则所有后续分配的部分将是 也分配给这些段，除非它们使用显式修饰符。你可以用它来告诉 linker 根本不将该部分放在任何段中。\`:phdr'`:phdr``:NONE`

下面是一个简单的示例：

```
PHDRS { text PT_LOAD ; }
SECTIONS { .text : { *(.text) } :text }
```

#### [输出部分填充](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC35)

您可以使用 来设置整个截面的填充图案。 是一个表达式 （请参阅[链接器脚本中的表达式](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC40)部分）。任何其他未指定的内存区域 在输出部分（例如，由于所需的 输入部分的对齐方式）将填充两个最少的 值的重要字节，必要时重复。\`=fillexp'fillexp

您还可以使用命令更改填充值 输出部分命令;请参阅[“输出部分数据](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC27)”部分。`FILL`

下面是一个简单的示例：

```
SECTIONS { .text : { *(.text) } =0x9090 }
```

### [叠加层说明](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC36)

叠加描述提供了一种描述部分的简单方法，这些部分 将作为单个内存映像的一部分加载，但将在 相同的内存地址。在运行时，某种覆盖管理器将 将覆盖的部分复制到运行时内存地址中和从运行时内存地址中复制出来 需要，也许通过简单地操作寻址位。这种方法 例如，当内存的某个区域速度更快时，它可能很有用 比另一个。

使用该命令描述叠加。该命令在命令中使用，例如 输出部分说明。该命令的完整语法如下：`OVERLAY``OVERLAY``SECTIONS``OVERLAY`

```
OVERLAY [<var>start</var>] : [NOCROSSREFS] [AT ( <var>ldaddr</var> )]
  {
    <var>secname1</var>
      {
        <var>output-section-command</var>
        <var>output-section-command</var>
        ...
      } [:<var>phdr</var>...] [=<var>fill</var>]
    <var>secname2</var>
      {
        <var>output-section-command</var>
        <var>output-section-command</var>
        ...
      } [:<var>phdr</var>...] [=<var>fill</var>]
    ...
  } [&gt;<var>region</var>] [:<var>phdr</var>...] [=<var>fill</var>]
```

除了（关键字）之外，一切都是可选的，并且每个 部分必须有一个名称（及以上）。这 构造中的节定义与 那些在一般结构中（参见 [SECTIONS 命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC17)部分）， 除了不能定义任何地址和内存区域 .`OVERLAY`secname1secname2`OVERLAY``SECTIONS``OVERLAY`

所有部分都使用相同的起始地址进行定义。负载 各部分的地址排列成连续的 内存从用于 AS 的加载地址开始 整体（与普通截面定义一样，加载地址是可选的， 并默认为起始地址;起始地址也是可选的， 并默认为位置计数器的当前值）。`OVERLAY`

如果使用了关键字，并且有任何引用 在这些部分中，链接器将报告错误。由于各节 都运行在同一个地址，通常对一个地址没有意义 部分直接引用另一个。请参阅[其他链接器脚本命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC13)部分。`NOCROSSREFS`

对于 中的每个部分，链接器自动 定义两个符号。符号是 定义为截面的起始荷载地址。该符号定义为 该部分。其中任何不合法的字符 在 C 中，标识符被删除。C（或汇编程序）代码可以使用这些 符号，以便根据需要移动叠加部分。`OVERLAY``__load_start_secname``__load_stop_secname`secname

在覆盖的末尾，位置计数器的值设置为 叠加的起始地址加上最大截面的大小。

下面是一个示例。请记住，这将出现在构造中。`SECTIONS`

```
  OVERLAY 0x1000 : AT (0x4000)
   {
     .text0 { o1/*.o(.text) }
     .text1 { o2/*.o(.text) }
   }
```

这将定义两者，并从 地址0x1000。 将在地址 0x4000 处加载，并将在 之后立即加载。这 将定义以下符号：、。\`.text0'\`.text1'\`.text0'\`.text1'\`.text0'`__load_start_text0``__load_stop_text0``__load_start_text1``__load_stop_text1`

将叠加层复制到叠加层区域的 C 代码可能看起来 如下所示。`.text1`

```
  extern char __load_start_text1, __load_stop_text1;
  memcpy ((char *) 0x1000, &amp;__load_start_text1,
          &amp;__load_stop_text1 - &amp;__load_start_text1);
```

请注意，该命令只是语法糖，因为 它所做的一切都可以使用更基本的命令来完成。以上 示例可以按如下方式编写。`OVERLAY`

```
  .text0 0x1000 : AT (0x4000) { o1/*.o(.text) }
  __load_start_text0 = LOADADDR (.text0);
  __load_stop_text0 = LOADADDR (.text0) + SIZEOF (.text0);
  .text1 0x1000 : AT (0x4000 + SIZEOF (.text0)) { o2/*.o(.text) }
  __load_start_text1 = LOADADDR (.text1);
  __load_stop_text1 = LOADADDR (.text1) + SIZEOF (.text1);
  . = 0x1000 + MAX (SIZEOF (.text0), SIZEOF (.text1));
```

## [MEMORY 命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC37)

链接器的默认配置允许分配所有可用配置 记忆。您可以使用命令覆盖此命令。`MEMORY`

该命令描述了块的位置和大小 目标中的内存。您可以使用它来描述哪些内存区域 可以由链接器使用，以及它必须避免哪些内存区域。你 然后可以将部分分配给特定的内存区域。链接器将 根据内存区域设置部分地址，并会警告 变得太满的区域。链接器不会随机播放部分 周围以适应可用区域。`MEMORY`

链接器脚本最多可以包含该命令的一次用法。但是，您可以在其中定义任意数量的内存块 你希望。语法为：`MEMORY`

```
MEMORY 
  {
    <var>name</var> [(<var>attr</var>)] : ORIGIN = <var>origin</var>, LENGTH = <var>len</var>
    ...
  }
```

这是链接器脚本中使用的名称，用于引用 地区。区域名称在链接器脚本之外没有任何含义。 区域名称存储在单独的命名空间中，不会发生冲突 使用符号名称、文件名或节名称。每个内存区域 必须具有不同的名称。name

该字符串是指定 是否将特定的内存区域用于输入部分，该输入部分是 未在链接器脚本中显式映射。如中所述 section [SECTIONS 命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC17)（如果未为某些输入指定输出部分） 部分，链接器将创建一个与 输入部分。如果定义区域属性，链接器将使用 它们选择它创建的输出部分的内存区域。attr

字符串必须仅包含以下字符：attr

\`R'

只读部分

\`W'

读/写部分

\`X'

可执行部分

\`A'

可分配部分

\`I'

初始化部分

\`L'

同\`I'

\`!'

反转上述任何属性的意义

如果未映射的部分与除 之外的任何列出的属性匹配，则该部分将被放置在内存区域中。该属性将反转此测试，以便放置未映射的部分 仅当它与列出的任何 属性。\`!'\`!'

是内存起始地址的表达式 地区。表达式的计算结果必须在内存之前为常量 执行分配，这意味着您不能使用任何部分 相对符号。关键字可以缩写为 or（但不能缩写，例如，）。origin`ORIGIN``org``o``ORG`

是内存大小（以字节为单位）的表达式 地区。与表达式一样，表达式必须 在执行内存分配之前计算为常量。这 关键字可以缩写为 或 。lenorigin`LENGTH``len``l`

在以下示例中，我们指定有两个内存区域 可供分配：一个起价为 256 KB， 另一个从四兆字节开始。这 链接器会将每个部分放入内存区域 未显式映射到内存区域，并且是只读的 或可执行文件。链接器将放置其他未放置的部分 显式映射到内存区域到内存中 地区。\`0'\`0x40000000'\`rom'\`ram'

```
MEMORY 
  {
    rom (rx)  : ORIGIN = 0, LENGTH = 256K
    ram (!rx) : org = 0x40000000, l = 4M
  }
```

定义内存区域后，可以将链接器定向到 使用 output section 属性将特定输出部分添加到该内存区域中。例如，如果您有 一个名为 的内存区域，您将在 输出部分定义。请参阅[“输出”部分区域](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC33)部分。如果没有地址 为输出部分指定，链接器会将地址设置为 内存区域中的下一个可用地址。如果结合 定向到内存区域的输出部分太大，无法 region，链接器将发出错误消息。\`>region'\`mem'\`>mem'

## [PHDRS 命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC38)

ELF 对象文件格式使用**程序头**，也称为**段**。程序标头描述程序应该如何 加载到内存中。您可以使用带有该选项的程序打印出来。`objdump`\`-p'

当您在原生 ELF 系统上运行 ELF 程序时，系统加载程序 读取程序头，以便弄清楚如何加载 程序。这只有在程序头设置正确时才有效。 本手册没有描述系统加载程序如何的详细信息 解释程序头;有关详细信息，请参阅 ELF ABI。

默认情况下，链接器将创建合理的程序头。然而 在某些情况下，您可能需要更多地指定程序头 正是。您可以为此目的使用该命令。什么时候 链接器在链接器脚本中看到该命令，它将 不创建指定标头以外的任何程序标头。`PHDRS``PHDRS`

链接器仅在以下情况下关注命令 生成 ELF 输出文件。在其他情况下，链接器将简单地 忽视。`PHDRS``PHDRS`

这是命令的语法。单词 、 、 和 是关键字。`PHDRS``PHDRS``FILEHDR``AT``FLAGS`

```
PHDRS
{
  <var>name</var> <var>type</var> [ FILEHDR ] [ PHDRS ] [ AT ( <var>address</var> ) ]
        [ FLAGS ( <var>flags</var> ) ] ;
}
```

仅用于命令中的引用 链接器脚本。它不会放入输出文件中。程序 标头名称存储在单独的命名空间中，不会发生冲突 使用符号名称、文件名或节名称。每个程序头 必须具有不同的名称。name`SECTIONS`

某些程序头类型描述的内存段 系统加载器将从文件加载。在链接器脚本中，您 通过放置可分配的输出来指定这些段的内容 段中的部分。您可以使用输出部分 属性，将部分放置在特定段中。请参阅[输出部分 phdr](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC34)。\`:phdr'

将某些部分放在多个段中是正常的。这 仅暗示一段记忆包含另一段记忆。你可以 重复 ，对每个应该 包含该部分。\`:phdr'

如果使用 ，将一个部分放置在一个或多个段中，则使用 然后链接器将放置所有后续的可分配部分，这些部分执行 未在同一段中指定。这是为了 方便，因为通常一整套连续的部分将是 放置在单个段中。您可以使用 默认段，并告诉链接器不要将该段放在任何 完全没有段。\`:phdr'\`:phdr'`:NONE`

您可以使用 和 关键字出现在 程序标头类型，用于进一步描述段的内容。 关键字表示该区段应包括 ELF 文件头。关键字表示细分应该 包括 ELF 程序标头本身。`FILEHDR``PHDRS``FILEHDR``PHDRS`

可能是以下情况之一。数字表示 关键字的值。type

`PT_NULL`(0)

指示未使用的程序头。

`PT_LOAD`(1)

指示此程序标头描述要从中加载的段 文件。

`PT_DYNAMIC`(2)

指示可以找到动态链接信息的区段。

`PT_INTERP`(3)

指示程序解释器的名称可能为 发现。

`PT_NOTE`(4)

指示保存注释信息的段。

`PT_SHLIB`(5)

保留程序标头类型，由 ELF 定义但未指定 阿比。

`PT_PHDR`(6)

指示可以找到程序头的段。

expression

给出程序头的数值类型的表达式。这可能 用于上面未定义的类型。

您可以指定应在特定地址加载段 在内存中使用表达式。这与用作输出部分属性的命令相同（请参阅[输出部分 LMA](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC32) 部分）。程序标头的命令将覆盖 output section 属性。`AT``AT``AT`

链接器通常会根据部分设置段标志 其中构成了该细分市场。您可以使用关键字来 显式指定段标志。的值必须为 整数。它用于设置程序的字段 页眉。`FLAGS`flags`p_flags`

下面是 的示例。这显示了一组典型的程序 本机 ELF 系统上使用的标头。`PHDRS`

```
PHDRS
{
  headers PT_PHDR PHDRS ;
  interp PT_INTERP ;
  text PT_LOAD FILEHDR PHDRS ;
  data PT_LOAD ;
  dynamic PT_DYNAMIC ;
}

SECTIONS
{
  . = SIZEOF_HEADERS;
  .interp : { *(.interp) } :text :interp
  .text : { *(.text) } :text
  .rodata : { *(.rodata) } /* defaults to :text */
  ...
  . = . + 0x1000; /* move to a new page in memory */
  .data : { *(.data) } :data
  .dynamic : { *(.dynamic) } :data :dynamic
  ...
}
```

## [VERSION 命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC39)

使用 ELF 时，链接器支持符号版本。符号版本包括 仅在使用共享库时有用。动态链接器可以使用 符号版本，用于在函数运行时选择函数的特定版本 可能已链接到早期版本的程序 共享库。

您可以直接在主链接器脚本中包含版本脚本，或者 可以将版本脚本作为隐式链接器脚本提供。您可以 另请使用链接器选项。\`--version-script'

该命令的语法很简单`VERSION`

```
VERSION { version-script-commands }
```

版本脚本命令的格式与 Solaris 2.5 中的 Sun 链接器。版本脚本定义了 版本节点。您可以在 版本脚本。您可以指定哪些符号绑定到哪些符号 版本节点，您可以将一组指定的符号简化为本地 范围，以便它们在共享之外不全局可见 图书馆。

演示版本脚本语言的最简单方法是使用一些 例子。

```
VERS_1.1 {
 global:
 foo1;
 local:
 old*; 
 original*; 
 new*; 
};

VERS_1.2 {
 foo2;
} VERS_1.1;

VERS_2.0 {
 bar1; bar2;
} VERS_1.2;
```

此示例版本脚本定义了三个版本节点。第一个 定义的版本节点是 ;它没有其他依赖项。 该脚本将符号绑定到 。它减少了 许多符号到局部范围，以便它们在外面不可见 共享库。\`VERS\_1.1'\`foo1'\`VERS\_1.1'

接下来，版本脚本定义节点。此节点 取决于。该脚本将符号绑定到 version 节点。\`VERS\_1.2'\`VERS\_1.1'\`foo2'\`VERS\_1.2'

最后，版本脚本定义节点。此节点 取决于。脚本绑定符号并绑定到 version 节点。\`VERS\_2.0'\`VERS\_1.2'\`bar1'\`bar2'\`VERS\_2.0'

当链接器找到库中定义的符号时，该符号不是 专门绑定到一个版本节点，它会有效地将其绑定到 库的未指定基本版本。您可以以其他方式绑定所有内容 未指定的符号到给定版本节点，方法是使用版本脚本中的某个位置。\`global: \*'

版本节点的名称除了什么之外没有特定的含义 他们可能会向阅读它们的人提出建议。版本 也可以出现在 和 之间。 但是，这将是编写版本脚本的一种令人困惑的方式。\`2.0'\`1.1'\`1.2'

将应用程序链接到已进行版本控制的共享库时 符号，应用程序本身知道每个符号的哪个版本 需要，它还知道每个节点需要哪个版本节点 它所链接的共享库。因此，在运行时，动态 Loader 可以进行快速检查，以确保您拥有的库 链接到 do 实际上提供了所有版本节点 应用程序将需要解析所有动态符号。在这个 动态链接器可以确定地知道 它需要的所有外部符号都可以解析，而不必 搜索每个符号参考。

符号版本控制实际上是一种更复杂的方式 执行 SunOS 所做的次要版本检查。根本问题 这里要解决的是，通常引用外部 函数是根据需要绑定的，并且在以下情况下并非所有绑定都绑定 应用程序将启动。如果共享库已过期，则 所需的接口可能缺失;当应用程序尝试使用 那个接口，它可能会突然意外地失败。带符号 版本控制，如果出现以下情况，用户在启动程序时将收到警告 与应用程序一起使用的库太旧了。

Sun 的版本控制方法有几个 GNU 扩展。这 首先是能够将符号绑定到 版本节点 定义符号的源文件，而不是在版本控制中定义符号 脚本。这样做主要是为了减轻图书馆的负担 保持。你可以通过输入类似的东西来做到这一点：

```
__asm__(".symver original_foo,foo@VERS_1.1");
```

在 C 源文件中。这会将函数重命名为 是绑定到 version 节点的别名。 该指令可用于防止导出符号。\`original\_foo'\`foo'\`VERS\_1.1'\`local:'\`original\_foo'

第二个 GNU 扩展是允许相同的多个版本 函数显示在给定的共享库中。这样你就可以 在不增加主要值的情况下对接口进行不兼容的更改 共享库的版本号，同时仍允许应用程序 链接到旧接口以继续运行。

为此，您必须在 源文件。下面是一个示例：\`.symver'

```
__asm__(".symver original_foo,foo@");
__asm__(".symver old_foo,foo@VERS_1.1");
__asm__(".symver old_foo1,foo@VERS_1.2");
__asm__(".symver new_foo,foo@@VERS_2.0");
```

在此示例中，表示绑定到 符号的未指定基本版本。包含此内容的源文件 示例将定义 4 个 C 函数：、、 和 。\`foo@'\`foo'\`original\_foo'\`old\_foo'\`old\_foo1'\`new\_foo'

当您对给定符号有多个定义时，需要有 指定外部引用的默认版本的某种方法 此符号将被绑定。您可以使用指令类型来执行此操作。你只能 以这种方式将符号的一个版本声明为默认值;否则 实际上，您将拥有同一符号的多个定义。\`foo@@VERS\_2.0'\`.symver'

如果希望将引用绑定到符号的特定版本 在共享库中，您可以使用“方便”的别名 （即 ），或者您可以使用指令 专门绑定到相关函数的外部版本。\`old\_foo'\`.symver'

## [链接器脚本中的表达式](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC40)

链接器脚本语言中表达式的语法与 C 表达式的表达式。所有表达式的计算结果均为整数。都 表达式的计算大小相同，即 32 位，如果两个 主机和目标为 32 位，否则为 64 位。

您可以在表达式中使用和设置符号值。

链接器定义了几个特殊用途的内置函数，用于 表达 式。

- [常量](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC41)：常量
- [符号](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC42)：符号名称
- [位置计数器](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC43)：位置计数器
- [运算符](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC44)：运算符
- [评估](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC45)：评估
- [表达式部分](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC46)：表达式的部分
- [内置函数](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC47)：内置函数

### [常数](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC41)

所有常量都是整数。

与 C 语言一样，链接器认为以 octal 和以 或 to be 开头的整数 十六进制。链接器将其他整数视为十进制。\`0'\`0x'\`0X'

此外，您可以使用后缀并缩放 常量由 分别。例如，以下所有内容都是指相同的数量：`K``M`

```
  _fourk_1 = 4K;
  _fourk_2 = 4096;
  _fourk_3 = 0x1000;
```

### [符号名称](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC42)

除非加引号，否则符号名称以字母、下划线或句点开头 并且可能包括字母、数字、下划线、句点和连字符。 不带引号的符号名称不得与任何关键字冲突。您可以 指定包含奇数字符或与 关键字，用双引号将符号名称括起来：

```
  "SECTION" = 9;
  "with a space" = "also with a space" + 10;
```

由于符号可以包含许多非字母字符，因此它是最安全的 用空格分隔符号。例如，是一个符号， 而是一个涉及减法的表达式。\`A-B'\`A - B'

### [位置计数器](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC43)

特殊链接器变量 **dot** 始终包含 电流输出位置计数器。由于 always 指的是 位置，它可能只出现在表达式中 在命令中。可能会出现符号 表达式中允许使用普通符号的任何位置。\`.'`.``SECTIONS``.`

赋值 to 将导致位置计数器为 搬。这可用于在输出部分创建孔。这 位置计数器可能永远不会向后移动。`.`

```
SECTIONS
{
  output :
    {
      file1(.text)
      . = . + 1000;
      file2(.text)
      . += 1000;
      file3(.text)
    } = 0x1234;
}
```

在前面的示例中，“file1”中的部分是 位于输出部分的开头。是的 后跟一个 1000 字节的间隙。然后出现“file2”中的部分，在“file3”中的部分后面还有一个 1000 字节的间隙。表示法指定要在间隙中写入的数据（请参阅[“输出”部分填充](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC35)部分）。\`.text'\`output'\`.text'\`.text'\`= 0x1234'

### [运营商](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC44)

链接器可识别标准 C 集算术运算符，其中 标准绑定和优先级： { @obeylines@parskip=0pt@parindent=0pt @dag@quad 前缀运算符。 @ddag@quad 请参阅[为符号赋值](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC14)部分。 }

### [评估](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC45)

链接器延迟计算表达式。它只计算 绝对必要时的表达式。

链接器需要一些信息，例如开始的值 第一部分的地址，以及内存的来源和长度 区域，以便进行任何链接。这些值是计算的 当链接器读取链接器脚本时，尽快。

但是，其他值（如符号值）是未知的，也不需要 直到存储分配之后。这些值稍后会评估，当 其他信息（例如输出部分的大小）可用 用于符号赋值表达式。

在分配之前无法知道部分的大小，因此 依赖于这些的分配直到之后才会执行 分配。

某些表达式（例如依赖于位置计数器的表达式）必须在部分分配期间进行计算。\`.'

如果表达式的结果是必需的，但值不是 可用，则会导致错误。例如，像 以后

```
SECTIONS
  {
    .text 9+this_isnt_constant : 
      { *(.text) }
  }
```

将导致错误消息。\`non constant expression for initial address'

### [表达式的部分](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC46)

When the linker evaluates an expression, the result is either absolute or relative to some section. A relative expression is expressed as a fixed offset from the base of a section.

The position of the expression within the linker script determines whether it is absolute or relative. An expression which appears within an output section definition is relative to the base of the output section. An expression which appears elsewhere will be absolute.

A symbol set to a relative expression will be relocatable if you request relocatable output using the option. That means that a further link operation may change the value of the symbol. The symbol's section will be the section of the relative expression. \`-r'

A symbol set to an absolute expression will retain the same value through any further link operation. The symbol will be absolute, and will not have any particular associated section.

You can use the builtin function to force an expression to be absolute when it would otherwise be relative. For example, to create an absolute symbol set to the address of the end of the output section : `ABSOLUTE`\`.data'

```
SECTIONS
  {
    .data : { *(.data) _edata = ABSOLUTE(.); }
  }
```

If were not used, would be relative to the section. \`ABSOLUTE'\`\_edata'\`.data'

### [Builtin Functions](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC47)

The linker script language includes a number of builtin functions for use in linker script expressions.

`ABSOLUTE(exp)`

Return the absolute (non-relocatable, as opposed to non-negative) value of the expression . Primarily useful to assign an absolute value to a symbol within a section definition, where symbol values are normally section relative. See section [The Section of an Expression](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC46). exp

`ADDR(section)`

Return the absolute address (the VMA) of the named . Your script must previously have defined the location of that section. In the following example, and are assigned identical values: section`symbol_1``symbol_2`

```
SECTIONS { ...
  .output1 :
    { 
    start_of_output_1 = ABSOLUTE(.);
    ...
    }
  .output :
    {
    symbol_1 = ADDR(.output1);
    symbol_2 = start_of_output_1;
    }
... }
```

`ALIGN(exp)`

Return the location counter () aligned to the next boundary. must be an expression whose value is a power of two. This is equivalent to doesn't change the value of the location counter--it just does arithmetic on it. Here is an example which aligns the output section to the next byte boundary after the preceding section and sets a variable within the section to the next boundary after the input sections: The first use of in this example specifies the location of a section because it is used as the optional attribute of a section definition (see section [Output section address](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC20)). The second use of is used to defines the value of a symbol. The builtin function is closely related to . `.`expexp

```
(. + <var>exp</var> - 1) &amp; ~(<var>exp</var> - 1)
```

`ALIGN``.data``0x2000``0x8000`

```
SECTIONS { ...
  .data ALIGN(0x2000): {
    *(.data)
    variable = ALIGN(0x8000);
  }
... }
```

`ALIGN`address`ALIGN``NEXT``ALIGN`

`BLOCK(exp)`

这是 的同义词，用于与较旧的链接器兼容 脚本。在设置输出地址时最常看到它 部分。`ALIGN`

`DEFINED(symbol)`

如果在链接器全局符号表中，则返回 1 并且 定义，否则返回 0。您可以使用此函数提供 符号的默认值。例如，以下脚本片段 演示如何将全局符号设置为 该部分 - 但如果一个符号已经调用了 存在，其值保留：symbol\`begin'\`.text'\`begin'

```
SECTIONS { ...
  .text : {
    begin = DEFINED(begin) ? begin : . ;
    ...
  }
  ...
}
```

`LOADADDR(section)`

返回名为 的绝对 LMA。这通常是 与 相同，但如果在输出部分定义中使用该属性，则可能会有所不同（请参阅[输出部分 LMA](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC32) 部分）。section`ADDR``AT`

`MAX(exp1, exp2)`

返回 和 的最大值。exp1exp2

`MIN(exp1, exp2)`

返回 和 的最小值。exp1exp2

`NEXT(exp)`

返回下一个未分配的地址，该地址是 的倍数。 此函数与 ;除非你 使用命令为 输出文件，则这两个函数是等效的。exp`ALIGN(exp)``MEMORY`

`SIZEOF(section)`

如果该部分有 ，则返回 named 的大小（以字节为单位）。 已分配。如果该部分尚未分配，则此时 评估后，链接器将报告错误。在以下示例中，并分配了相同的值：section`symbol_1``symbol_2`

```
SECTIONS{ ...
  .output {
    .start = . ;
    ...
    .end = . ;
    }
  symbol_1 = .end - .start ;
  symbol_2 = SIZEOF(.output);
... }
```

`SIZEOF_HEADERS`

`sizeof_headers`

返回输出文件标头的大小（以字节为单位）。这是 出现在输出文件开头的信息。您可以使用 这个数字在设置第一节的起始地址时，如果你 选择，以便于分页。 生成 ELF 输出文件时，如果链接器脚本使用内置函数，则链接器必须计算 在确定所有部分之前的程序头数 地址和大小。如果链接器稍后发现它需要 附加程序头，它将报告错误。若要避免此错误，必须避免使用 函数，否则必须重新设计链接器 脚本以避免强制链接器使用其他程序头，或者 您必须使用该命令自行定义程序头（请参阅 [PHDRS 命令](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html#SEC38)部分）。`SIZEOF_HEADERS`\`not enough room for program headers'`SIZEOF_HEADERS``PHDRS`

## [隐式链接器脚本](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html#TOC48)

如果指定链接器无法识别的链接器输入文件 一个目标文件或一个存档文件，它将尝试将文件读取为 链接器脚本。如果文件无法解析为链接器脚本，则 链接器将报告错误。

隐式链接器脚本不会替换默认链接器脚本。

通常，隐式链接器脚本仅包含符号 赋值，或者 、 或 命令。`INPUT``GROUP``VERSION`

将读取由于隐式链接器脚本而读取的任何输入文件 在命令行中隐式链接器脚本所在的位置 读。这可能会影响存档搜索。

___

转到[第一](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_1.html)节、[上](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_2.html)一节、[下](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_4.html)一节、[最后一](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_8.html)节，[目录](https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_toc.html)。