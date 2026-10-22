# Breakout

Classic Breakout game built with Qt6 and DTK6.

## Requirements

- Qt 6 (Widgets module)
- Dtk6Widget

### Ubuntu/Debian

```bash
sudo apt install qt6-base-dev libdtk6widget-dev
```

### Arch Linux

```bash
sudo pacman -S qt6-base dtk6
```

## Compile

```bash
cd Breakout
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Run

```bash
./breakout
```

## Controls

- **Flechas izquierda/derecha**: Mover la paleta
- **Mouse**: Mover la paleta con el cursor
- **Espacio**: Lanzar la pelota
- **P**: Pausa
- **Ctrl+N**: Nuevo juego
- **Ctrl+Q**: Salir
- **Enter**: Reiniciar tras game over

## Scoring

Los ladrillos de las filas superiores dan mas puntos:

| Fila | Color | Puntos |
|------|-------|--------|
| 1 | Rojo | 50 |
| 2 | Naranja | 40 |
| 3 | Amarillo | 30 |
| 4 | Verde | 20 |
| 5 | Azul | 10 |

Tienes 3 vidas. Pierdes una vida cuando la pelota cae por debajo de la paleta.
