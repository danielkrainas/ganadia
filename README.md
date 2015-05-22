# Ganadia

Ganadia is a multi-user dungeon(MUD) server, based on CB:SC. The code is currently in a state of repair and restoration as it's based on an old surviving backup.

Some of the short-term goals are:
- Use MongoDB for game data storage.
    - Create abstract data module that will replace the many `fread_*` calls.
- Restore turn-based combat system from SMAUG.
- Remove non-essential/non-administrative OLC commands and plan a web-based control interface as a separate project.
- Lua or python script support for commands/spells/skills.