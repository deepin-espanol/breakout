# Breakout

Rompe todos los ladrillos con la pelota sin dejarla caer. Desarrollado con **Qt6** y **Deepin Tool Kit (DTK6)** para Deepin Linux.

## Características

- Niveles: al vaciar el muro empieza otro, la pelota acelera y ganas una vida extra
- Récord de puntos guardado
- Control con teclado o ratón
- Pausa automática al cambiar de ventana
- Interfaz DTK que sigue el tema claro/oscuro; traducido a 8 idiomas

## Controles

| Tecla | Acción |
|-------|--------|
| ← / → | Mover la pala |
| Ratón | Mover la pala |
| Espacio / clic | Lanzar la pelota |
| P / Esc | Pausa |
| Enter | Reiniciar tras perder |
| Ctrl+N | Nuevo juego |
| Ctrl+Q | Salir |

## Instalación

Descarga el paquete de la [última release](https://github.com/deepin-espanol/breakout/releases/latest):

- `.deb` (amd64, arm64, riscv64, loong64):
  ```bash
  sudo apt install ./breakout_*.deb
  ```
- `.uab` de linyaps (amd64, arm64): ejecutable directamente.
  ```bash
  chmod +x org.deepinenespanol.*.uab && ./org.deepinenespanol.*.uab
  ```

## Compilar

```bash
sudo apt build-dep ./          # dependencias declaradas en debian/control
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
./build/breakout
```

O generar el paquete `.deb`:

```bash
dpkg-buildpackage -us -uc -b
```

## Publicar una versión

Añade una entrada nueva en `debian/changelog` y haz push: el workflow
*Build and publish .deb* crea la release con los `.deb` y los `.uab`.

## Licencia

GPL-3.0 o posterior. © Deepin en Español — https://deepinenespanol.org
