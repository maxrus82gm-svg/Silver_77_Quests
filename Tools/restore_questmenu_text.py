#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Скрипт для безопасного восстановления повреждённых text-строк в QuestMenu.layout

ВАЖНО:
- Скрипт работает только с файлом Silver_77_Quests_Client/gui/QuestMenu.layout
- Перед изменением создаётся резервная копия с расширением .bak
- Изменяются только значения text у указанных виджетов
- Структура layout-файла не меняется
- Кодировка файла сохраняется (cp1251)

ИСПОЛЬЗОВАНИЕ:
    python Tools/restore_questmenu_text.py

ТРЕБОВАНИЯ:
    Python 3.6+
"""

import os
import shutil
import re
from pathlib import Path

# Карта восстановления текста
# Ключ: имя виджета, Значение: новый текст
TEXT_RESTORE_MAP = {
    'TitleText': 'КВЕСТЫ',
    'DescriptionPanelLabel': 'Описание квеста',
    'DescriptionText': 'Выберите квест из списка',
    'RoutePanelLabel': 'Маршрут',
    'DialogPanelLabel': 'Диалог NPC',
    'AcceptButtonText': 'ВЗЯТЬ КВЕСТ',
    'CompleteButtonText': 'СДАТЬ КВЕСТ',
    'CloseButtonText': 'Закрыть рот и уйти',
}

# Путь к целевому файлу
TARGET_FILE = 'Silver_77_Quests_Client/gui/QuestMenu.layout'

# Кодировка файла
ENCODING = 'cp1251'


def create_backup(file_path):
    """Создаёт резервную копию файла с расширением .bak"""
    backup_path = file_path + '.bak'
    shutil.copy2(file_path, backup_path)
    print(f"✓ Резервная копия создана: {backup_path}")
    return backup_path


def read_layout_file(file_path, encoding):
    """Читает layout-файл с указанной кодировкой"""
    try:
        with open(file_path, 'r', encoding=encoding) as f:
            content = f.read()
        print(f"✓ Файл прочитан: {file_path} (кодировка: {encoding})")
        return content
    except UnicodeDecodeError as e:
        print(f"✗ Ошибка чтения файла с кодировкой {encoding}: {e}")
        raise
    except FileNotFoundError:
        print(f"✗ Файл не найден: {file_path}")
        raise


def replace_widget_text(content, widget_name, new_text):
    """
    Заменяет значение text у указанного виджета
    
    Ищет блок виджета по имени класса и заменяет строку text внутри него.
    Использует регулярное выражение для точечной замены.
    """
    # Паттерн для поиска виджета и его text-значения
    # Ищем: WidgetClass WidgetName { ... text "старое значение" ... }
    pattern = rf'(\b\w+WidgetClass\s+{re.escape(widget_name)}\s+\{{[^}}]*?\s+text\s+)"([^"]*)"'
    
    def replacer(match):
        return match.group(1) + f'"{new_text}"'
    
    new_content, count = re.subn(pattern, replacer, content, flags=re.DOTALL)
    
    return new_content, count


def restore_text_values(file_path, text_map, encoding):
    """
    Восстанавливает text-значения у виджетов согласно карте
    
    Возвращает:
        tuple: (успешно изменено, не найдено)
    """
    # Читаем файл
    content = read_layout_file(file_path, encoding)
    
    # Создаём резервную копию
    create_backup(file_path)
    
    # Применяем замены
    changed_widgets = []
    not_found_widgets = []
    
    for widget_name, new_text in text_map.items():
        new_content, count = replace_widget_text(content, widget_name, new_text)
        
        if count > 0:
            content = new_content
            changed_widgets.append(widget_name)
            print(f"✓ Виджет '{widget_name}': text изменён на '{new_text}'")
        else:
            not_found_widgets.append(widget_name)
            print(f"⚠ Виджет '{widget_name}': не найден в файле")
    
    # Записываем изменённый файл
    with open(file_path, 'w', encoding=encoding, newline='\n') as f:
        f.write(content)
    
    print(f"\n✓ Файл сохранён: {file_path} (кодировка: {encoding})")
    
    return changed_widgets, not_found_widgets


def main():
    """Главная функция скрипта"""
    print("=" * 80)
    print("СКРИПТ ВОССТАНОВЛЕНИЯ TEXT-СТРОК В QuestMenu.layout")
    print("=" * 80)
    print()
    
    # Проверяем существование файла
    if not os.path.exists(TARGET_FILE):
        print(f"✗ ОШИБКА: Файл не найден: {TARGET_FILE}")
        print("Убедитесь, что скрипт запущен из корня проекта Silver_77_Quests")
        return 1
    
    print(f"Целевой файл: {TARGET_FILE}")
    print(f"Кодировка: {ENCODING}")
    print(f"Виджетов для восстановления: {len(TEXT_RESTORE_MAP)}")
    print()
    
    # Запрашиваем подтверждение
    response = input("Продолжить восстановление? (yes/no): ").strip().lower()
    if response not in ['yes', 'y', 'да']:
        print("Операция отменена пользователем.")
        return 0
    
    print()
    print("-" * 80)
    print("НАЧАЛО ВОССТАНОВЛЕНИЯ")
    print("-" * 80)
    print()
    
    try:
        # Выполняем восстановление
        changed, not_found = restore_text_values(TARGET_FILE, TEXT_RESTORE_MAP, ENCODING)
        
        # Выводим итоги
        print()
        print("-" * 80)
        print("ИТОГИ ВОССТАНОВЛЕНИЯ")
        print("-" * 80)
        print(f"✓ Успешно изменено виджетов: {len(changed)}")
        if changed:
            for widget in changed:
                print(f"  - {widget}")
        
        if not_found:
            print(f"\n⚠ Не найдено виджетов: {len(not_found)}")
            for widget in not_found:
                print(f"  - {widget}")
        
        print()
        print("=" * 80)
        print("ВОССТАНОВЛЕНИЕ ЗАВЕРШЕНО")
        print("=" * 80)
        print()
        print("РЕКОМЕНДАЦИИ:")
        print("1. Откройте файл в редакторе и проверьте, что кириллица отображается")
        print("2. Проверьте, что структура файла не изменилась")
        print("3. Запустите игру и проверьте отображение UI")
        print(f"4. Если что-то пошло не так, восстановите из резервной копии: {TARGET_FILE}.bak")
        print()
        
        return 0
        
    except Exception as e:
        print()
        print("=" * 80)
        print("✗ ОШИБКА ПРИ ВОССТАНОВЛЕНИИ")
        print("=" * 80)
        print(f"Тип ошибки: {type(e).__name__}")
        print(f"Сообщение: {e}")
        print()
        print("Файл не был изменён или изменён частично.")
        print(f"Проверьте резервную копию: {TARGET_FILE}.bak")
        return 1


if __name__ == '__main__':
    exit(main())
