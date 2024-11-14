# Робот Сумо на Arduino Uno

## Описание проекта

Этот проект представляет собой робота-сумо, управляемого микроконтроллером Arduino Uno. Робот оснащен двумя двигателями для передвижения и ультразвуковым датчиком расстояния для обнаружения цели. Контроль двигателей осуществляется с помощью драйвера L298N. Робот предназначен для участия в соревнованиях по сумо-роботам, где основная задача — обнаружить и загнать соперника за пределы арены.

## Использованные компоненты

- **Микроконтроллер**: Arduino Uno
- **Датчики**:
  - Ультразвуковой датчик расстояния (например, HC-SR04)
- **Механика**:
  - 2 электрических двигателя постоянного тока (DC)
- **Электроника**:
  - Драйвер двигателей L298N
- **Элементы питания**:
  - 6 батареек (для обеспечения суммарного напряжения 9V)
- **Прочее**:
  - Провода для соединений
  - Шасси для робота
  - Колеса

## Схема подключения

![Figura-5-Circuito-logico-del-funcionamiento-completo-del-Robot-aspiradora-Hybrid-AV](https://github.com/user-attachments/assets/5e0bd6db-366e-46ad-801f-2df0fc5b5994)



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

4. **Элементы питания**:
   - Используйте 6 батареек для обеспечения стабильного напряжения 9V.
   - Подключите положительный вывод к `12V` на L298N, а отрицательный к `GND`.

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
