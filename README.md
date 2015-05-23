# Ganadia

Ganadia is a multi-user dungeon(MUD) server, based on CB:SC. The code is currently in a state of repair and restoration as it's based on an old surviving backup.

Some of the current goals are:
- Use MongoDB for game data storage.
    - Create abstraction for data IO operations.
- Restore turn-based combat system from SMAUG.
- Remove non-essential/non-administrative OLC commands and plan a web-based control interface as a separate project.
	- Possible use of libwebsockets to intercommunicate changes and updates between systems.
- Lua or python script support for commands/spells/skills.
- Extract content into its own project.
- Windows support.
	- Create abstraction for network layer.
- Localization support.
- MUD extension protocol.
- MUD sound support.
- Markdown support for help files.
- Virtual/temp room support.