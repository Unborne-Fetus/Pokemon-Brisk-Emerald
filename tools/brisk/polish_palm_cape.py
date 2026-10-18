#!/usr/bin/env python3
"""Rebuild Route 145 as two tropical islands linked by a scenic boardwalk.
Retains the map size, connections, scripts, encounters, and existing NPC identities.
Run explicitly from the repository root; normal map checks never rebuild it.
"""
from orange_islands import *


def rebuild():
    old = next(a for a in current_areas() if a.name == 'Route145')
    assert (old.w, old.h) == (48, 36), 'Review the design before resizing Palm Cape.'
    a = Area(old.name, old.w, old.h)
    a.meta = old.meta
    ellipse(a.land, 11, 20, 9, 11)
    ellipse(a.land, 36, 17, 10, 12)
    for y in range(a.h):
        for x in range(a.w):
            if x < 3 or x > 45:
                a.land[y][x] = False
    ellipse(a.green, 11, 20, 6, 8)
    ellipse(a.green, 36, 16, 7, 8)
    for y in range(a.h):
        for x in range(a.w):
            a.green[y][x] &= a.land[y][x]
    a.ground()

    # Three clear boardwalk lanes line up with Orange City's east approach
    # and Route 146's west shore. The middle bridge crosses open water.
    a.boardwalk(0, 19, 6, 3)
    a.boardwalk(17, 19, 13, 3)
    a.boardwalk(43, 19, 5, 3)
    a.boardwalk(34, 25, 3, 8)
    a.boardwalk(32, 29, 7, 3)

    # Encounter groves leave a broad central promenade and several quiet
    # clearings. Native grass quadrants retain the normal encounter behavior.
    for y in range(a.h):
        for x in range(a.w):
            west = 6 <= x <= 15 and 13 <= y <= 18
            east = 35 <= x <= 41 and 9 <= y <= 15
            if (west or east) and a.grid[y][x] & 1023 == GRASS:
                a.grid[y][x] = block([[0xA, 0xB], [0xC, 0xD]][y % 2][x % 2])

    # Actual three-tile-high island palms, matching the beach palms on 144.
    palms = [(4, 15), (7, 10), (15, 12), (4, 24), (15, 27),
             (28, 13), (31, 7), (40, 7), (43, 13), (41, 24), (29, 23)]
    for x, y in palms:
        if all(a.land[y+dy][x+dx] for dy in range(3) for dx in range(2)):
            a.stamp(x, y, [[0x12, 0x13], [0x1A, 0x1B], [0x22, 0x23]])
    for x, y in [(8, 13), (12, 15), (37, 10), (39, 13)]:
        a.stamp(x, y, [[0x1E, 0x1F], [0x26, 0x27]])

    # Resting garden on the west island and the Hex Maniac's east lookout.
    for x, y in [(9, 24), (32, 16)]:
        a.stamp(x, y, [[0x22E, 0x22F]])
        for xx in (x-1, x+2):
            a.grid[y][xx] = block(5)
    for x, y in [(7, 22), (15, 22), (30, 20), (40, 20)]:
        a.grid[y-1][x] = block(0xF7)
        a.grid[y][x] = block(0xFF, True)
    for x, y in [(7, 19), (9, 19), (11, 19), (13, 19), (15, 19),
                 (7, 22), (10, 22), (13, 22), (16, 22),
                 (8, 26), (11, 26), (13, 25),
                 (30, 18), (32, 18), (34, 18), (36, 18), (38, 18), (40, 18),
                 (30, 22), (32, 22), (38, 22), (40, 22), (32, 12), (33, 12)]:
        if a.grid[y][x] & 1023 in (GRASS, 5, 0x10):
            a.grid[y][x] = block(5 if (x+y) % 3 else 0x10)

    for event in a.meta['bg_events']:
        if event.get('script') == 'Route145_EventScript_RouteSign':
            event['x'], event['y'] = 14, 24
            a.grid[24][14] = block(3, True)
    for obj in a.meta['object_events']:
        if obj.get('script') == 'Route145_EventScript_HexManiac':
            obj['x'], obj['y'] = 32, 14
        elif obj.get('script') == 'Route145_EventScript_Cape':
            obj['x'], obj['y'] = 35, 30
        a.grid[obj['y']][obj['x']] = block(
            a.grid[obj['y']][obj['x']] & 1023, False)

    audit_palm_cape(a)
    write_words('data/layouts/Route145/map.bin', [v for row in a.grid for v in row])
    save('data/maps/Route145/map.json', a.meta)
    render(current_areas())


if __name__ == '__main__':
    rebuild()
