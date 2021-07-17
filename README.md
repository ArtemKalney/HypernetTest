# HypernetTest

В работе разрабатывались алгоритмы расчёта коэффицентов полиномов надёжности показателей связности вторичных сетей двухуровневых гиперсетей, показывающих их зависимость от вероятности присутствия рёбер ("ветвей", в терминалогии гиперсетей) в первичной сети. Выбраны показатели, основанные на вероятности связности пары узлов вторичной сети при возможных разрушениях сети первичной.

# Гранч

Решалась задача надёжности сети электропитания в шахте при возникновении аварийной ситуации (в данном случае обрушения выработки, ветви в теории гиперсетей). Сеть питания состоит из 3-х уровней (АОШ, КП и БС, см. подробнее в введении). АОШ имеет надёжное (двухфидерное) подключение к КП и их количество совпадает в районе порядка как можно видеть из предложенных примеров. Поэтому рассматривается уровень подключения КП к БС. Каждое подключение представляет дерево. При решении задачи предполагалось эти деревья не имеют пересечений, что значительно ускорило вычисления. 

Итоговая задача - расчёт суммы MENC по 7 выделенным вершинам из 8-ми возможных. Первичная сеть представлена на рис.:
![origPic](https://user-images.githubusercontent.com/22174649/126031578-92453858-9c19-4d8b-b894-8169ae854b6e.png)

Недочёты:
1. Не очень ясно описано что даёт результат конечному пользователю.
2. Не выполнено требование по несовпадению точек пересечения выработок и точек подключения.


# Зависимости

Для распараллеливания вычислений используется библиотека [MS-MPI](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi?redirectedfrom=MSDN). Для сериализации-десериализации сообщений между рабочими процессами была использована библиотека [boost](https://nuwen.net/mingw.html).
