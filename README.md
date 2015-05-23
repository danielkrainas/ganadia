# Ganadia

Ganadia is a multi-user dungeon(MUD) server, based on CB:SC. The code is currently in a state of repair and restoration as it's based on an old surviving backup.

Some of the current goals are:
- [Use MongoDB for game data storage](mongo_milestone).
- Restore turn-based combat system from SMAUG.
- Remove non-essential/non-administrative OLC commands and plan a web-based control interface as a separate project.
	- Possible use of libwebsockets to intercommunicate changes and updates.
- Lua or python script support for commands/spells/skills.
- Extract content into its own project.
- Windows support.
	- Create abstraction for network layer.
- Localization support.
- MUD extension protocol.
- MUD sound support.
- Markdown support for help files.
- Virtual/temp room support.
- Use stat and damage calculations more similar to FFXI.

[mongo_milestone]: https://github.com/danielkrainas/ganadia/milestones/MongoDB%20Game%20Data%20Storage