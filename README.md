# Bloodlust Software Archive

A preservation repository for a source release from **Bloodlust Software**, a DOS and Windows 95-era developer best known here for **TimeSlaughter** and **Noggin Knockers 2**.

> **Provenance.** In October 2024, trapexit contacted Icer Addis to ask whether he would be willing to release the source code for Bloodlust Software projects. Icer responded with a ZIP file containing source code, executables, and assets for numerous titles and tools. This repository preserves the resulting material.

The supplied archive describes itself as a **1993–1996** collection of Bloodlust's non-emulator software, built with Watcom for DOS and Windows 95. It contains 9,093 non-Finder-metadata files (about 123 MiB) and 33 ZIP archives. All ZIP files were tested readable while preparing this README.

## Start Here

The complete supplied release lives in [`BLS-archive-199X/`](BLS-archive-199X/). It has deliberately retained its original broad organization:

- [`readme.txt`](BLS-archive-199X/readme.txt) — the archive's original two-line description.
- [`LICENSE.txt`](BLS-archive-199X/LICENSE.txt) — the supplied license text.
- [`199X-F/`](BLS-archive-199X/199X-F/) — release-era game data, documentation, music, install/configuration utilities, and source archives.
- [`Classic/`](BLS-archive-199X/Classic/) — larger source/build snapshots for TimeSlaughter and A32.
- [`source/`](BLS-archive-199X/source/) — development trees, prior snapshots, tools, experiments, and additional archived source packages.

## What Is Included

### TimeSlaughter

[`199X-F/`](BLS-archive-199X/199X-F/) is the largest game-data area: 5,053 files (about 45 MiB). It holds release-era material for TimeSlaughter, including DOS executables, installers, configuration utilities, MIDI music, source archives, and a large body of sprite, animation, sound, and move-list data.

Important locations:

- [`199X-F/TSDOC/`](BLS-archive-199X/199X-F/TSDOC/) — tray-card, insert, and font material.
- [`199X-F/MIDI/`](BLS-archive-199X/199X-F/MIDI/) — MIDI tracks and the retained `CAKE.ZIP` music-project archive.
- [`199X-F/XCUTE/`](BLS-archive-199X/199X-F/XCUTE/) — character, boss, and level assets.
- Named asset trees such as [`ASYLUM/`](BLS-archive-199X/199X-F/ASYLUM/), [`CHI/`](BLS-archive-199X/199X-F/CHI/), [`LAZARUS/`](BLS-archive-199X/199X-F/LAZARUS/), [`PORTAL/`](BLS-archive-199X/199X-F/PORTAL/), [`SAVAGE/`](BLS-archive-199X/199X-F/SAVAGE/), and [`VLAD/`](BLS-archive-199X/199X-F/VLAD/) — per-character/content animation, artwork, movement, and sound resources.
- [`Classic/ts/`](BLS-archive-199X/Classic/ts/) — a substantial TimeSlaughter source/build snapshot with Watcom project files, C/C++ source, assembly, object files, maps, and executables.
- [`source/tdemo/`](BLS-archive-199X/source/tdemo/) and [`source/OLDTD/`](BLS-archive-199X/source/OLDTD/) — additional, separately preserved TimeSlaughter development snapshots.

The supplied Bloodlust web pages in [`source/bldlust/bhome/`](BLS-archive-199X/source/bldlust/bhome/) describe TimeSlaughter as a PC fighting game and document its mail-order release context.

### Noggin Knockers 2

Noggin Knockers 2 material is concentrated under [`source/bldlust/`](BLS-archive-199X/source/bldlust/):

- [`noggin/`](BLS-archive-199X/source/bldlust/noggin/) — assembled game data, including `.VOL` files, DOS executables, fonts, title/select/ending assets, and the original game readme.
- [`nog/`](BLS-archive-199X/source/bldlust/nog/) and [`nogdemo/`](BLS-archive-199X/source/bldlust/nogdemo/) — source and Watcom build snapshots for the game and demo, including C/C++, assembly, linker inputs, object files, maps, and executables.
- [`nd/`](BLS-archive-199X/source/bldlust/nd/) — the demo-build/game-data directory and `N2DEMO.ZIP`.
- [`bhome/`](BLS-archive-199X/source/bldlust/bhome/) — original Bloodlust web pages, GIFs, thumbnails, and related art.

The source identifies the game as *Noggin Knockers 2* and contains 1996 Bloodlust copyright notices. The website material also documents its bundling with TimeSlaughter.

### A32 Animator and Related Technology

A32 is represented by its source, legacy versions, tools, and DOS/Windows 95 variants:

- [`Classic/a32/`](BLS-archive-199X/Classic/a32/) — the most complete A32 source/build tree, with `ados`, `a95w`, `a95b`, `win95`, and related platform-specific directories.
- [`source/A95SRC/`](BLS-archive-199X/source/A95SRC/), [`source/a32old/`](BLS-archive-199X/source/a32old/), and [`source/OLDA32/`](BLS-archive-199X/source/OLDA32/) — distinct historical A32 snapshots.
- [`source/OLDNET/`](BLS-archive-199X/source/OLDNET/) and [`source/ddnet/`](BLS-archive-199X/source/ddnet/) — networking-related development snapshots.
- [`source/A95.ZIP`](BLS-archive-199X/source/A95.ZIP), [`source/OBJ.ZIP`](BLS-archive-199X/source/OBJ.ZIP), and [`source/WALL.ZIP`](BLS-archive-199X/source/WALL.ZIP) — related source/archive packages.

The A32 source includes an “About A32” dialog and DOS/Windows 95 Watcom build configurations; it is tooling rather than a standalone game release.

### Grunt, Utilities, and Experiments

The release also includes smaller projects and shared development work:

- [`source/GRUNT/`](BLS-archive-199X/source/GRUNT/) — a sizeable project tree containing its own executable, data volumes, asset directories, documentation, and internal ZIP archives.
- [`source/gtest/`](BLS-archive-199X/source/gtest/) — graphics-generation/test work, including `g68` and `DISASM68` subprojects.
- [`source/file/`](BLS-archive-199X/source/file/) — file I/O and compression experiments, including LZH, LZSS, and LZW-related sources and test data.
- [`source/prof/`](BLS-archive-199X/source/prof/) and [`source/profr2/`](BLS-archive-199X/source/profr2/) — profiling-tool snapshots.
- [`source/tkv/`](BLS-archive-199X/source/tkv/) — a small `view` utility with source, project files, executable, map, and symbol file.
- [`source/MESSAGE/`](BLS-archive-199X/source/MESSAGE/) — the `MESSAGE.CPP` / `MESSAGE.H` component.

## Formats and Build Context

This is a historical source archive, not a modernized build. Expect DOS and Windows 95 development artifacts alongside sources:

- **Source and assembly:** `.C`, `.CPP`, `.H`, `.ASM`
- **Watcom projects/build inputs:** `.WPJ`, `.TGT`, `.MK`, `.MK1`, `.LK1`
- **Build products and diagnostics:** `.EXE`, `.OBJ`, `.MAP`, `.SYM`, `.ERR`, `.LST`
- **Game/media data:** `.VOL`, `.RAW`, `.BBM`, `.LBM`, `.RVC`, `.SND`, `.MID`, `.AWE`, `.CL2`
- **Preserved packages:** `.ZIP`

The original archive includes generated objects, executables, temporary files, backups, and multiple source states. Those are retained because they can be useful evidence when reconstructing the historical build environment or comparing revisions.

## Preservation Notes

- Treat the supplied directory names and case as archival data. Several projects rely on DOS-era paths and filenames.
- Similar files in different snapshots may intentionally be repeated; the snapshots document separate points in development.
- Retained ZIPs may overlap expanded directories, but remain useful as original distribution or point-in-time packages.
- The included source and assets are subject to the terms in [`BLS-archive-199X/LICENSE.txt`](BLS-archive-199X/LICENSE.txt). The presence of this repository does not add rights beyond that license.

## Repository Scope

This repository currently preserves the source release as received. It does not claim that every project builds on a contemporary host, nor does it replace the original Watcom/DOS/Windows 95 environment documented by the archive.
