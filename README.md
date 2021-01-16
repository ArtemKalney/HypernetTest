# HypernetTest

В работе разрабатывались алгоритмы расчёта коэффицентов полиномов надёжности показателей связности вторичных сетей двухуровневых гиперсетей, показывающих их зависимость от вероятности присутствия рёбер ("ветвей", в терминалогии гиперсетей) в первичной сети. Выбраны показатели, основанные на вероятности связности пары узлов вторичной сети при возможных разрушениях сети первичной.

# Зависимости

Для распараллеливания вычислений используется библиотека [MS-MPI](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi?redirectedfrom=MSDN). Для сериализации json была использована библиотека [boost](https://stackoverflow.com/questions/36519453/setup-boost-in-clion).
