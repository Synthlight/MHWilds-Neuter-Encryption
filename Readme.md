# Install
Extract the DLL so it winds up in REF's plugin dir, like so: `reframework\plugins\Neuter_Encryption.dll`

# Troubleshooting
This will create a log files right next to the EXE. (Same name, just extension changed from DLL to LOG.)

If it's not making a log, then REF probably isn't installed.<br>
If you've got a log, and it's working, it'll look something like this:
```
2026-01-09 18:36:39	Initializing.
2026-01-09 18:36:39	Attaching proxy.
2026-01-09 18:36:44	Module base address: 140000000
2026-01-09 18:36:44	Module size: 23CDF000
2026-01-09 18:36:44
2026-01-09 18:36:44	Scanning for Neuter Dahlia Float Encrypt bytes.
2026-01-09 18:36:44	Found 1 match(es).
2026-01-09 18:36:44	Inject address: 1400541B0 (MonsterHunterWilds.exe + 541B0)
2026-01-09 18:36:44	New mem bytes: 49,8B,00,48,89,01,49,8B,40,08,48,89,41,08,48,8B,C1,C3
2026-01-09 18:36:44
2026-01-09 18:36:44	Scanning for Neuter Dahlia Float Decrypt bytes.
2026-01-09 18:36:47	Found 1 match(es).
2026-01-09 18:36:47	Inject address: 14A43F020 (MonsterHunterWilds.exe + A43F020)
2026-01-09 18:36:47	New mem bytes: 49,8B,00,48,89,01,49,8B,40,08,48,89,41,08,48,8B,C1,C3
2026-01-09 18:36:47
2026-01-09 18:36:47	Patching done!
```

# Build
Clone https://github.com/Synthlight/Base-Dll-Proxy so it winds up in the same parent dir as this.<br>
e.g.:
```
\Plugin Mods\Base-Dll-Proxy
\Plugin Mods\Neuter-Encryption
```
Then just open this and build. This does have pre-set post-build copy commands that will probably fail on your system.

# Credits
Thanks to cola for finding & creating the AoB scans and replacement bytes.<br>
All I've (LordGregory) done is just put them in a plugin to patch the assy on startup.