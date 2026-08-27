# Shadowfront: English Dialogue Bible

## Voice Direction

The dialogue is concise, operational, and human. No character speaks in heroic slogans or imitates dialogue from an existing game. **Marwan Haddad** is controlled and observant; **Lian Qasim** is analytical but never detached from the stakes; **Yasser Nouri** is decisive and protective. Enemy transmissions use calm, procedural language that makes their actions feel more unsettling than shouted threats.

| Character | Function | Performance direction |
| --- | --- | --- |
| Marwan Haddad | Player character and field lead. | Low, restrained, breath-aware under pressure. |
| Lian Qasim | Comms analyst and remote mission support. | Clear, fast, technically precise; grows personal in the final operation. |
| Yasser Nouri | Squad commander in the field. | Grounded, direct, protective; never theatrical. |
| Silent Warden | Opposition relay voice. | Filtered, controlled, almost polite. |

## Opening Cinematic — “Ash Line”

The screen opens on black. A weather radar scans once, then tears into static. A low harbor horn emerges beneath rain. The **Sami** mark resolves briefly in oxidized radar green, then fades into an aerial view of a sleeping port.

| Cue | Speaker | English line |
| --- | --- | --- |
| 00:00 | Harbor radio | “Harbor control to all inbound traffic. Hold position until the lights return.” |
| 00:09 | Lian | “That message has been repeating for forty-three minutes. It isn’t coming from harbor control.” |
| 00:18 | Yasser | “The grid is dead. Emergency boats are grounded. Someone wanted the city blind.” |
| 00:28 | Marwan | “Then we give it its eyes back.” |
| 00:35 | Lian | “Your entry point is Pier Seven. Find the relay recorder. Do not destroy it.” |
| 00:44 | Yasser | “One clean route in. No support until the generators are live.” |
| 00:52 | Marwan | “Copy. Moving.” |

The camera drops through the rain toward a shipping corridor and blends into the player’s helmet view. The first playable input appears with the objective: **REACH PIER SEVEN**.

## Operation 01 — Fog Line

| Trigger | Speaker | English line |
| --- | --- | --- |
| First movement | Lian | “Keep low. The fog is carrying sound tonight.” |
| Player spots first hostile | Marwan | “Contact. One guard, center lane.” |
| Generator reached | Lian | “That crane runs on an isolated circuit. Bring it up and I can map the port.” |
| First enemy wave | Yasser | “They heard the generator. Hold the lane.” |
| Generator restored | Lian | “Grid fragment online. I’ve got cameras—no, someone is overwriting the feeds.” |
| Recorder discovered | Marwan | “This isn’t cargo traffic. It’s an evacuation schedule.” |
| Enemy broadcast | Silent Warden | “Civilian movement is an acceptable variable. Interference is not.” |
| Operation end | Lian | “The signal points inland. Old desalination plant. They’re using the pipes as a backbone.” |

## Operation 02 — Steel Echo

| Trigger | Speaker | English line |
| --- | --- | --- |
| Entry tunnel | Yasser | “Water level is rising. If the doors lock, we do not get a second exit.” |
| Pump objective | Lian | “Three pumps. Stabilize all of them or the lower plant floods.” |
| Hostiles flank | Marwan | “Movement left. They’re trying to turn us into the open.” |
| Drone appears | Lian | “That drone is tagging your position. Take the operator first.” |
| Defending Lian | Yasser | “Qasim needs one more minute. Buy it.” |
| Truth revealed | Lian | “The distress calls were manufactured. They emptied the waterfront before taking the relay network.” |
| Marwan response | Marwan | “They didn’t steal a signal. They moved an entire city.” |
| Operation end | Silent Warden | “You restored a path. We only need one more.” |

## Operation 03 — Zero Line

| Trigger | Speaker | English line |
| --- | --- | --- |
| Dawn approach | Lian | “Sunrise in twelve minutes. The city will see the damage before we can explain it.” |
| First relay tower | Yasser | “Secure the tower. No shortcuts. If Atlas wakes wrong, every rescue channel goes dark again.” |
| Marwan backstory | Lian | “Pier Nine wasn’t your failure, Marwan. You stayed when everyone else was told to leave.” |
| Marwan response | Marwan | “I stayed too long.” |
| Second tower | Silent Warden | “You call it restoration because you cannot admit how fragile it was.” |
| Escort phase | Yasser | “I’m at the core door. Keep the route clear and don’t let them pin us here.” |
| Final choice preparation | Lian | “I can shut Atlas down, or route it through their relay and trace whoever built this. Either way, you have seconds.” |
| Final combat | Marwan | “Then make the seconds count.” |
| Safe ending | Lian | “Atlas is stable. The harbor is answering again.” |
| Trace ending | Lian | “I have a source. Not a name—yet. But it’s enough to start.” |
| Final line | Marwan | “No more blind cities.” |

## Combat Barks and System Calls

| Event | English line |
| --- | --- |
| Low health | “Armor’s gone. Staying mobile.” |
| Reload | “Reloading.” |
| Final rounds | “Magazine’s nearly empty.” |
| Confirmed hostile | “Hostile down.” |
| Objective updated | “New task. Mark it.” |
| Ammo cache found | “Supply case. That helps.” |
| Flanking warning | “They’re pushing around the cover.” |
| Mission checkpoint | “Checkpoint set.” |
| Difficulty: Recon | “Recon protocol active. Navigation support online.” |
| Difficulty: Operation | “Operation protocol active. Standard response.” |
| Difficulty: Veteran | “Veteran protocol active. No assistance.” |

## Audio Implementation Notes

Dialogue should use dry close-mic source for direct lines and filtered radio variants for support and enemy transmissions. Critical navigation instructions must remain intelligible under music and combat; therefore, music ducks briefly under priority dialogue and returns through a short recovery curve. The first APK slice uses this complete script as a source library but records only the opening and Operation 01 lines required by the playable content, then expands on the same system as later operations become playable.
