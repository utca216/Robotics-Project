# Робот Сумо на Arduino Uno

## Описание проекта

Этот проект представляет собой робота-сумо, управляемого микроконтроллером Arduino Uno. Робот движется с помощью двух двигателей, управляемых драйвером L298N, и обнаруживает препятствия с помощью ультразвукового датчика. Он также оснащен двумя датчиками цвета, которые помогают ему оставаться на ринге, реагируя на черные линии. Если робот наезжает на черную линию передним датчиком, он отъезжает назад; если задним — движется вперед. Дополнительно установлен сервомотор, который активирует привод механизма захвата при обнаружении препятствия на расстоянии менее 5 см.

## Использованные компоненты

- **Микроконтроллер**: Arduino Uno
- **Датчики**:
  - Ультразвуковой датчик расстояния (например, HC-SR04)
  - Два датчика цвета — цифровые, для определения черной линии
- **Механика**:
  - 2 электрических двигателя постоянного тока (DC)
  - Сервомотор — для срабатывания захватывающего механизма при близком препятствии
- **Электроника**:
  - Драйвер двигателей L298N
- **Элементы питания**:
  - 6 батареек (для обеспечения суммарного напряжения 9V)
- **Прочее**:
  - Провода для соединений
  - Шасси для робота
  - Колеса

## Схема подключения

![Figura-5-Circuito-logico-del-funcionamiento-completo-del-Robot-aspiradora-Hybrid-AV](https://github.com/user-attachments/assets/7410ea0f-ebf1-4300-9545-c178297da527)




### Подключение компонентов:

1. **Arduino Uno**:
   - Подключите пины управления двигателями к пинам `IN1`, `IN2`, `IN3`, `IN4` на модуле L298N.
   - Подключите пины `ENA` и `ENB` к ШИМ-пинам Arduino для управления скоростью моторов (например, пины 6 и 7).
   - Подключите ультразвуковой датчик:
     - `TRIG` к пину 8 Arduino
     - `ECHO` к пину 9 Arduino

2. **Драйвер двигателей L298N**:
   - Подключите выводы `IN1`-`IN4` к соответствующим пинам Arduino для управления направлением вращения моторов.
   - Подключите выводы `ENA` и `ENB` к ШИМ-пинам Arduino для регулировки скорости.
   - Подключите питание:
     - `12V` к источнику питания (в данном случае 9V от батареек)
     - `GND` к общему заземлению с Arduino

3. **Датчик расстояния**:
   - Подключите `VCC` к 5V на Arduino
   - Подключите `GND` к общему заземлению
   - Подключите `TRIG` и `ECHO` к соответствующим цифровым пинам Arduino

4. **Сервомотор**:
   - Питание (VCC) подключено к пину 11.
   - Земля (GND) подключена к пину 12.
   - Управляющий сигнал подключен к пину 10 Arduino.

5.**Датчики цвета**:
   - Передний датчик цвета подключен к цифровому пину 13 Arduino.
   - Задний датчик цвета подключен к цифровому пину 14 Arduino.

5. **Элементы питания**:
   - Используйте 6 батареек для обеспечения стабильного напряжения 9V.
   - Подключите положительный вывод к `12V` на L298N, а отрицательный к `GND`.
## Логика работы

1. **Движение и поиск цели**:
   - Робот вращается, сканируя пространство ультразвуковым датчиком.
   - Когда датчик обнаруживает цель (расстояние менее 255 см), робот начинает двигаться вперед.
   - Если цель исчезает, робот делает поворот сначала вправо, затем влево. Если цель не обнаружена, робот продолжает крутиться влево.

2. **Реакция на препятствия**:
   - При обнаружении цели на расстоянии менее 5 см сервомотор поворачивается на 60 градусов.

3. **Реакция на черные линии**:
   - Если передний датчик цвета обнаруживает черную линию, робот отъезжает назад на определенное время.
   - Если задний датчик цвета обнаруживает черную линию, робот движется вперед на определенное время.

## Логика работы кода

1. **Поиск цели**:
   - Робот начинает вращаться в поисках цели, постоянно измеряя расстояние с помощью ультразвукового датчика.

2. **Обнаружение цели**:
   - Если расстояние до цели меньше 255 см, робот перестает вращаться и начинает двигаться вперед к цели.

3. **Потеря цели**:
   - Если цель теряется (расстояние становится больше 255 см), робот выполняет поворот вправо на определенное время.
   - После поворота вправо снова проверяет наличие цели.
     - Если цель не обнаружена, выполняет поворот влево на заданное время.
     - Если и после поворота влево цель не найдена, продолжает вращаться влево в поисках цели.

4. **Реакция на близкое препятствие**:
   - При обнаружении цели ближе 5 см сервомотор поворачивается на 60 градусов.

5. **Реакция на черные линии**:
   - Если передний датчик цвета обнаруживает черную линию, робот отъезжает назад на определенное время.
