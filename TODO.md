**Saker som bör göras - 150427**

Dokument generellt
- Kandidatrapporten	(Dennis skriver inledning, syfte, frågeställningar)
- 			(Johan skriver teori (om active set, subproblem bl a))
- 			(Ruben skriver metodik)
- 			(Adam tar fortsatt arbete)
- 			(Sebastian tar forskningsmetod)
- 			(Martin tar datastrukturer)
- 			(Yngve tar gruppens gemensamma erfarenheter)
- 			
- Individuell del 	(Överens om 50 timmar var på de individuella delarna.)
- Teknisk dokumentation (Sebastian har koll på läget, får dirigera om vad som behövs göras i koden)
	
Solver
- Refactor är på gång. Antagligen klart, men värt om flera går igenom koden.
- Testa. Jämför med Gurobi och möjligtvis MATLAB samt CVXGEN. 
- Lös megagrejen, A och B => E och h matris.

Parser
- "Koppla" GUI delen med C-koden. Är i princip klart, bör testas dock.
- Parsern behöver kunna tolka Daniel Simons vilkor. Följ template.c som finns i GUI mappen.
- Sebastian, Adam och Dennis har ansvar för parsern.

MATLAB
- Klar annars. Vad vill Daniel Simon att funktionen ska returnera? Sebastian tror punkten.

**Krav som ej är uppfyllda**

- Ska kunna exekveras från en körbar kompilerad fil.
- GUI:t ska kunna anropa lösaren.
- Ska lösa problem lika snabbt eller snabbare än den kommersiella programvaran Gurobi.
- Programmet ska kunna hitta en lösning med specifierad noggrannhet.
- Programmet ska inte krascha vid specialfall eller felaktig indata.

**150513**

Utkast 2 av kandidatrapport (gemensam och individuell) ska in.

**150520**

Genrep, dokumentopposition
- Presentation	(Johan och Martin)
- Opponering	(Ruben, Yngve och Adam)

**150527**

- Kandidatrapporten ska in (gemensamm och individuell).
- Demonstration, kursavslut

