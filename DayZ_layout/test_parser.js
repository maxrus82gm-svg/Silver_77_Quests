// Тестовый скрипт для проверки парсера .layout файлов
const fs = require('fs');

// Функция парсера (аналогичная той, что в HTML)
function parseDayZLayout(content) {
    const lines = content.split('\n');
    const elements = [];
    
    let currentElement = null;
    let braceCount = 0;
    let inElement = false;

    for (let i = 0; i < lines.length; i++) {
        const line = lines[i].trim();
        
        // Отслеживаем открывающие фигурные скобки
        const openBraces = (line.match(/\{/g) || []).length;
        const closeBraces = (line.match(/\}/g) || []).length;
        
        braceCount += openBraces - closeBraces;
        
        // Ищем начало нового UI-элемента (класс с именем)
        const classMatch = line.match(/^\s*(\w+WidgetClass)\s+(\w+)\s*\{/);
        if (classMatch) {
            currentElement = {
                type: classMatch[1],
                name: classMatch[2],
                position: null,
                size: null,
                color: null,
                depth: braceCount - 1
            };
            inElement = true;
        }
        
        // Если мы внутри элемента, ищем его свойства
        if (inElement && currentElement) {
            // Парсим position (формат: position x y)
            const posMatch = line.match(/^\s*position\s+([\d\.]+)\s+([\d\.]+)/);
            if (posMatch) {
                currentElement.position = {
                    x: parseFloat(posMatch[1]) || 0,
                    y: parseFloat(posMatch[2]) || 0
                };
            }
            
            // Парсим size (формат: size w h)
            const sizeMatch = line.match(/^\s*size\s+([\d\.]+)\s+([\d\.]+)/);
            if (sizeMatch) {
                currentElement.size = {
                    w: parseFloat(sizeMatch[1]) || 0,
                    h: parseFloat(sizeMatch[2]) || 0
                };
            }
            
            // Парсим color (формат: color r g b a)
            const colorMatch = line.match(/^\s*color\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)/);
            if (colorMatch) {
                currentElement.color = {
                    r: parseFloat(colorMatch[1]) || 0,
                    g: parseFloat(colorMatch[2]) || 0,
                    b: parseFloat(colorMatch[3]) || 0,
                    a: parseFloat(colorMatch[4]) || 1
                };
            }
        }
        
        // Если встретили закрывающую скобку и вышли из текущего элемента
        if (closeBraces > 0 && currentElement && braceCount < currentElement.depth) {
            // Сохраняем элемент, если у него есть position и size
            if (currentElement.position && currentElement.size && 
                currentElement.size.w > 0 && currentElement.size.h > 0) {
                elements.push({...currentElement});
            }
            currentElement = null;
            inElement = false;
        }
    }
    
    return elements;
}

// Тестируем на реальном файле
const layoutContent = fs.readFileSync('../gui/QuestMenu.layout', 'utf8');
const elements = parseDayZLayout(layoutContent);

console.log(`Найдено элементов: ${elements.length}`);
console.log('\nПримеры найденных элементов:');
elements.slice(0, 5).forEach((el, i) => {
    console.log(`${i+1}. ${el.type} "${el.name}"`);
    console.log(`   position: ${el.position.x} ${el.position.y}`);
    console.log(`   size: ${el.size.w} ${el.size.h}`);
    if (el.color) {
        console.log(`   color: ${el.color.r} ${el.color.g} ${el.color.b} ${el.color.a}`);
    }
    console.log('');
});