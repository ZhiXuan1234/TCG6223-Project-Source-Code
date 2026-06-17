# State of Code — TCG6223 TPS Project
**Group C&A | Multimedia University | Last Updated: 2026-06-16**

---

## 1. Project Overview

A 3D OpenGL boss-fight game inspired by **The Amazing Digital Circus (TADC)**, built in C++ using the fixed-function OpenGL/GLUT pipeline.

The player controls **Kinger** — a fully animated Third-Person Shooter (TPS) character — in a circus-arena environment. The game's ultimate goal is a **boss fight against Caine**, the ringmaster AI, who spawns waves of **Gloinks** (arena-hazard enemies) as part of his attack pattern.

**Current Status:** Kinger's full TPS control system is feature-complete. The environment, Caine's model (all 14 parts), and all 6 Gloink variant meshes are loaded and textured but not yet wired into any active game logic. The next milestone is Phase 1 of the Caine boss fight.

---

## 2. Core Player Mechanics (Kinger) — Implemented & Active

### 2.1 Movement & Physics

- **WASD Movement** — Evaluated every frame in `Kinger::update()` via `GetAsyncKeyState` polling. Diagonal movement is correctly **normalised** to prevent the 41% speed boost from holding two keys simultaneously.
- **Camera-Relative Direction** — Forward (`-sin/cos(cameraYaw)`) and right (`cos/-sin(cameraYaw)`) vectors are computed from the live camera yaw so Kinger always moves relative to where the player is *looking*, not where the model is facing.
- **Gravity & Jump Physics** — Constant downward acceleration of `−30.0 units/s²` applied every frame. Spacebar fires a `+30.0 units/s` vertical impulse. Ground collision is detected at `posY = −18.7f`.
- **Squash & Stretch** — `jumpScaleY` scales the entire model vertically: stretches to `1.2×` on launch, squashes to `0.8×` on landing, and smoothly lerps back to `1.0×` at `10×` per second.
- **Roll Speed Boost** — During the dodge roll, `KINGER_INTERNAL_SPEED` is multiplied by `2.5×`.
- **Dynamic Body Leaning** — `currentLeanPitch` (forward/back tilt) smoothly lerps toward `targetLeanPitch` (`±15°`) using `LEAN_SPEED = 3.0`. The lean is applied to the **body group only**, not the arms, so the gun barrel stays locked to the crosshair during movement.

### 2.2 Camera & Aiming

- **TPS Follow Camera** — Rebuilt every frame in `myDisplayFunc()`. The camera orbits Kinger in a sphere controlled by `cameraYaw` and `cameraPitch`. Configured with a `60-unit` follow distance, `18-unit` right-shoulder offset, and a `30-unit` base height.
- **Mouse-Look** — `myPassiveMotionFunc()` accumulates mouse delta into `cameraYaw`/`cameraPitch` and warps the cursor to the window centre every frame for infinite mouse travel. Sensitivity: `0.003 rad/px`.
- **Pitch Clamping** — `cameraPitch` is hard-clamped to `[−1.2, +1.2]` radians (~69°) to prevent gimbal flip.
- **Smooth Camera Height Tracking** — `cameraTrackY` uses a lerp (`CAMERA_FOLLOW_SPEED = 10.0`) to smoothly follow Kinger's `posY` during jumps, preventing jarring vertical snaps.
- **3D Crosshair Rendering** — `drawCrosshair()` switches to a `gluOrtho2D` projection and draws a white `GL_LINES` cross at a fixed screen position (`cx = width/2 − 15`, `cy = height/2 + 30`). This offset accounts for the over-the-shoulder camera to make the crosshair visually converge with the gun barrel.

### 2.3 State Machine — Abilities

All four abilities are **mutually exclusive** and enforced via guard clauses at the top of each `castXxx()` function. The state machine lives in `KingerAnimation`.

---

#### 🔫 Shooting (`LMB` / Hold)

- **Trigger:** Left mouse click (`myMouseFunc`) fires once. Holding left mouse (`keyStates[1]` via `GetAsyncKeyState(VK_LBUTTON)`) enables auto-fire — checked every frame in `Kinger::update()`.
- **Ammo:** 30-round magazine (`MAX_AMMO = 30`). Each shot decrements `currentAmmo`. At zero rounds, `castReload()` is called automatically.
- **Recoil Animation:** A 2-phase linear animation. Phase 1 (0–0.05s): `armRecoilOffset` lerps from `0` to `+3.0` units (Z-axis push-back). Phase 2 (0.05–0.25s): ease-in lerp back to `0`. Re-fire is unlocked immediately after Phase 2.
- **Arm Isolation:** `armSwayWeight` fades to `0.0` at `15×/s` when shooting and recovers at `5×/s` when idle. This mathematically zeroes the gun's idle breathing sway during combat without stopping the rest of the body's animation.
- **Bullet Trajectory (3D Parallax):**
  1. On spawn, the bullet's world-space origin is computed by rotating the hand's local offset `(+16.25, 0, −5)` by the current `facingYaw` using a 2D rotation matrix, then pushing `BULLET_SPAWN_Z = −25` units along the aim direction.
  2. A pre-calculated normalised direction vector `(bulletDirX/Y/Z)` stores `(−sin(yaw)·cos(pitch), −sin(pitch), −cos(yaw)·cos(pitch))` at the moment of firing.
  3. Every frame, `BULLET_TRAVEL_SPEED = 250 units/s` is multiplied by `deltaTime` and added to position along this frozen direction vector. The bullet despawns at `BULLET_MAX_DISTANCE = 400 units`.
  4. A `GL_LINES` tracer is drawn from `bulletStartX/Y/Z` to the current `bulletPosX/Y/Z` in bright yellow-orange (`rgba 1.0, 0.8, 0.0, 0.6`) using alpha blending.

---

#### 🔄 Reloading (`R`)

- **Guard Clause:** Blocked if `isReloading`, `isRolling`, or `isHealing`.
- **Duration:** 1.0 seconds, 3 phases:
  - **Phase 1 (0–0.3s):** Left arm sweeps across the chest (`leftArmReloadPitch → −120°`, `leftArmReloadYaw → −135°`, `leftArmReloadYOffset → +10`). Right arm brings the gun inward (`rightArmReloadPitch → −60°`).
  - **Phase 2 (0.3–0.7s):** Both arms oscillate with `sin(reloadTimer * 30)` to simulate the magazine-swap action.
  - **Phase 3 (0.7–1.0s):** All offsets lerp back to `0`. On completion, `currentAmmo` is reset to `MAX_AMMO = 30`.
- **Aim Tracking:** During Phase 1/2, the left hand's pitch is blended with the gun's current aim angle, weighted by `leftArmReloadYaw / −135.0` so the hand naturally tracks the gun barrel as it sweeps in.

---

#### ⚽ Dodge Roll (`C`)

- **Guard Clause:** Blocked if `isCastingSkill`, `isRolling`, `isReloading`, or `isHealing`.
- **Duration:** 0.5 seconds, 4 phases:
  - **Phase 0 (0–0.1s):** `rollSquashY` lerps from `1.0` to `0.3` — the body crushes down.
  - **Phase 1 (0.1–0.4s):** `showBallModel = true` — the normal multi-part Kinger body is replaced by the `KingerRoll` sphere model. The ball spins on the X-axis at `rollTimer * 1500°`.
  - **Phase 2 (0.4–0.5s):** `showBallModel = false`, body reappears. `rollSquashY` lerps from `0.3` back to `1.0`.
  - **Phase 3 (>0.5s):** State reset.
- **Model Swap:** The roll model is drawn via `::myvirtualworld.kingerRoll.draw()` from inside `Kinger::draw()`, using the global world instance reference.

---

#### 🦋 Butterfly Heal (`F`)

- **Guard Clause:** Blocked if `isHealing`, `isRolling`, or `butterflyCharges <= 0`.
- **Charges:** 3 total (`butterflyCharges` starts at `100` — effectively unlimited for now). Each use decrements charges and instantly adds `+30 HP` to `currentHealth`, clamped at `maxHealth = 100`.
- **Left Arm Animation:** `leftArmHealPitch` lerps to `−180°` over 0.5s, holds for 1.0s, then returns to `0°` over 0.5s (total 2.0s).
- **Movement Lock:** `Kinger::update()` and `jump()` both check `animation.isHealing`; all movement input is ignored while healing.
- **Dynamic Butterfly Flight Path (drawn in `Kinger::draw()`):**
  - **Phase 1 (0–0.5s):** Butterfly is locked to the left hand's world-space position, computed by rotating the local hand offset `(−12, +15, −15)` by `facingYaw`.
  - **Phase 2 (0.5–1.5s):** Position lerps from the hand world position to the bucket world position (`posX/Z`, `posY + 18.7 + 25`).
  - **Phase 3 (1.5–2.0s):** Butterfly rests at the bucket. `scale` lerps from `1.0` to `0.0`, fading it out.
  - **Wing flapping:** `flapAngle = sin(healTimer * 20) * 30°` drives the `Butterfly::draw(flapAngle)` call.
- **Healing Particle Burst:** When `healTimer >= 1.5s`, 3 `GL_QUADS` 3D `+` crosses are rendered. Each cross rises (`height = crossTimer * 15`), orbits the bucket using `sin/cos(offsetTimer * 10 + i)`, slowly spins (`offsetTimer * 300°`), and fades out as it rises.

---

## 3. File-by-File Breakdown

### Core Game Loop

| File | Namespace | Responsibility |
|---|---|---|
| `CNAmain.cpp` | *(global)* | Entry point. Owns the GLUT event loop, all input callbacks, the TPS camera rebuild (`gluLookAt` every frame), `cameraYaw`/`cameraPitch` state, `keyStates[256]` array, `GetAsyncKeyState` polling, and the 2D crosshair draw call. |
| `CNAmain.hpp` | *(global)* | Framework structs (`MyWindow`, `MyWorld`, `MyViewer`, `MySetting`, `MyAxis`). Forward-declares all GLUT callbacks. **Do not modify.** |
| `CNAWorld.hpp` | `ProjectWorld` | Declares `MyVirtualWorld` — the single root object that owns every game entity as a public member (`kinger`, `gloinks`, `caine`, `butterfly`, `kingerRoll`, `environment`, `audioManager`). |
| `CNAWorld.cpp` | `ProjectWorld` | Implements `init()` (loads all `.txt` OBJ models and textures for every entity), `draw()` (calls `environment.draw()` and `kinger.draw()`), and `tickTime()` (computes frame `deltaTime`, calls `kinger.update()` and `environment.tickTime()`). Also owns `setupEnvironmentLighting()`. |

### Player Character

| File | Namespace | Responsibility |
|---|---|---|
| `Kinger.hpp` | `ProjectKinger` | Class definition for `Kinger`. Declares world position (`posX/Y/Z`, `facingYaw`), physics (`velocityY`, `isGrounded`, `jumpScaleY`), lean state (`currentLeanPitch/Roll`, `targetLeanPitch/Roll`, `MAX_LEAN_ANGLE`), health (`currentHealth`, `maxHealth`), texture IDs, and all per-part model/loaded-flag members. Owns the `KingerAnimation animation` member. |
| `Kinger.cpp` | `ProjectKinger` | Implements: `update()` (movement, diagonal normalisation, lean lerp, gravity, jump, state machine ticking), `jump()`, `moveForward/Right()` (legacy helpers), per-part `loadXxx()` and `drawXxx()` functions, the master `draw()` (world transform, model-swap branch, lean/arm group split, healing butterfly draw), and `drawBullet()` (tracer line + `gluSphere`). |
| `KingerAnimation.hpp` | *(global class)* | Plain data class (no namespace). Declares all animation state variables: idle (`idleTimer`, `hoverOffset`, `clothRotation`, `armRotation`, `armSwayWeight`), skill (`skillTimer`, `isCastingSkill`, `armRecoilOffset`), bullet (`isBulletActive`, `bulletPosX/Y/Z`, `bulletStartX/Y/Z`, `bulletDirX/Y/Z`, `bulletDistance`), roll (`isRolling`, `rollTimer`, `rollPhase`, `rollSquashY`, `showBallModel`), reload (`isReloading`, `reloadTimer`, arm offsets), and heal (`isHealing`, `healTimer`, `butterflyCharges`, `leftArmHealPitch`). Also holds the four bullet travel constants. |
| `KingerAnimation.cpp` | *(global class)* | Implements all six state-update functions: `updateIdleState()`, `updateSkillState()` (bullet spawn + direction vector + recoil phases), `updateRollState()` (4-phase squash/swap), `updateReloadState()` (3-phase arm kinematics), `updateHealState()` (3-phase arm lift), plus the two trigger functions `castGunSkill()`, `castRollSkill()`, `castReload()`, and `castHealSkill()`. |
| `KingerRoll.hpp` | `ProjectKingerRoll` | Minimal class for the ball-form mesh. Owns one `ObjModel rollModel`, `rollTextureID`, and `loadRoll()`/`draw()`. |
| `KingerRoll.cpp` | `ProjectKingerRoll` | Draws the ball: applies a `5.0×` uniform scale and `glRotatef(180, Y)` facing correction. Called from `Kinger::draw()` when `animation.showBallModel == true`. |

### Butterfly (Heal VFX)

| File | Namespace | Responsibility |
|---|---|---|
| `Butterfly.hpp` | `ProjectButterfly` | 3-part model (left wing, right wing, torso). Exposes `draw(float flapAngle)` — the angle drives per-wing rotation to animate flapping. |
| `Butterfly.cpp` | `ProjectButterfly` | Draws the three parts. Left and right wings are rotated by `±flapAngle` around their respective pivot axes to create the flapping motion. Called from `Kinger::draw()` during heal Phase 1–3. |

### Boss & Enemies (Models Loaded, Logic Pending)

| File | Namespace | Responsibility |
|---|---|---|
| `Caine.hpp` | `ProjectCaine` | 14-part character model (hat, torso, upper/lower jaw, tongue, left/right hand, palm, leg × 2, left/right eye, staff). All parts have individual `ObjModel`, texture ID, `loadXxx()`, and `drawXxx()` members. `draw()` is declared but currently commented out in `CNAWorld::draw()`. |
| `Caine.cpp` | `ProjectCaine` | Implements all 14 per-part load and draw functions. No animation or AI logic yet. |
| `Gloinks.hpp` | `ProjectGloinks` | 6-variant enemy model set: BowlingPin, Circle, Cube, Moon, Star, Triangular. Each variant has its own `ObjModel`, texture ID, and draw function. `draw()` is commented out in `CNAWorld::draw()`. |
| `Gloinks.cpp` | `ProjectGloinks` | Implements all load and draw functions for the 6 Gloink shapes. No spawning, movement, or collision logic yet. |

### Environment

| File | Namespace | Responsibility |
|---|---|---|
| `Environment.hpp` | `ProjectEnvironment` | Declares all arena models: skybox, ground, castle wall, cube, grouped-cube, irregular cube, pillar, roof, sphere. Also owns all texture IDs, the `animationTime` float (drives `tickTime()`), and `drawDigitalEffect()` for glitch animation. |
| `Environment.cpp` | `ProjectEnvironment` | Implements the full arena draw: layered skybox, ground plane, castle walls, decorative geometry, pillars, roof, and the digital glitch effect. `tickTime()` advances `animationTime` for environment animations. `loadTextures()` loads all environment texture maps via `TextureLoader`. |

### Utilities

| File | Namespace | Responsibility |
|---|---|---|
| `ObjModel.hpp/cpp` | *(global)* | Parses custom `.txt` OBJ-format model files into vertex/normal/UV arrays. Exposes `loadFromObjText(path)` and `draw()`. Used as the base mesh container for every character and environment part. |
| `TextureLoader.hpp/cpp` | *(global)* | `TextureLoader::loadTexture(path)` — a thin static helper that wraps `stb_image` to load `.jpg`/`.png` files and return an OpenGL `GLuint` texture ID. |
| `AudioManager.hpp/cpp` | *(global)* | `playBackgroundMusic(path)` wrapper (currently commented out in `CNAWorld::init()`). Placeholder for WINAPI/FMOD audio integration. |

---

## 4. Key Constants & Tuning Values

```cpp
// ── Camera (CNAmain.cpp) ─────────────────────────────────────────────────────
CAMERA_DISTANCE         = 60.0f   // Units behind Kinger
CAMERA_BASE_HEIGHT      = 30.0f   // Height above look-at target
CAMERA_SHOULDER_OFFSET  = 18.0f   // Right-shoulder shift
CAMERA_TARGET_HEIGHT_OFFSET = 5.0f // Kinger chest height above ground
CAMERA_PITCH_MAX/MIN    = ±1.2f   // Radians
MOUSE_SENSITIVITY       = 0.003f
CAMERA_FOLLOW_SPEED     = 10.0f   // Lerp speed for vertical tracking

// ── Bullet (KingerAnimation.hpp) ────────────────────────────────────────────
BULLET_TRAVEL_SPEED  = 250.0f  // World units per second
BULLET_SPAWN_Z       = -25.0f  // Forward distance from body at spawn
BULLET_MAX_DISTANCE  = 400.0f  // Despawn distance (total 3D travel)
BULLET_BARREL_Y_BIAS = -2.0f   // Vertical correction for barrel tip

// ── Movement & Physics (Kinger.cpp) ─────────────────────────────────────────
KINGER_INTERNAL_SPEED   = 45.0f   // World units per second
ROLL_SPEED_MULTIPLIER   = 2.5f
Gravity acceleration    = -30.0f  // Units/s²
Jump impulse            = +30.0f  // Units/s
Ground level            = -18.7f
MAX_LEAN_ANGLE          = 15.0f   // Degrees

// ── Healing ─────────────────────────────────────────────────────────────────
butterflyCharges (init) = 100     // Effectively unlimited; reduce for balance
HP per heal             = +30
```

---

## 5. Architecture: Render & Update Flow

```
glutMainLoop()
  └─ myDisplayFunc()                        [every frame]
       ├─ Rebuild gluLookAt (TPS camera)
       ├─ glPushMatrix()
       │    ├─ worldaxis.draw()
       │    └─ myvirtualworld.draw()
       │         ├─ environment.draw()      [arena, skybox, walls]
       │         └─ kinger.draw()           [body groups + bullet + heal VFX]
       └─ glPopMatrix()
       ├─ drawCrosshair()                   [2D ortho overlay]
       ├─ glFlush() / glutSwapBuffers()
       ├─ updateKeyStatesFromWindows()      [GetAsyncKeyState poll]
       └─ myvirtualworld.tickTime()
            ├─ compute deltaTime
            ├─ kinger.update(dt, yaw, pitch, keys)
            │    ├─ facingYaw = cameraYaw
            │    ├─ auto-fire check (keyStates[1])
            │    ├─ animation.updateIdleState(dt)
            │    ├─ animation.updateSkillState(dt, ...)
            │    ├─ animation.updateRollState(dt)
            │    ├─ animation.updateReloadState(dt)
            │    ├─ animation.updateHealState(dt)
            │    ├─ WASD movement + diagonal normalisation
            │    ├─ lean lerp
            │    └─ gravity + jump + floor collision
            └─ environment.tickTime()
```

---

## 6. Upcoming Milestones

### ✅ Phase 0 — Kinger TPS Core (COMPLETE)
All mechanics listed in Section 2 are implemented and stable.

---

### 🔲 Phase 1 — Caine Boss Fight (NEXT)

All of Caine's models and textures are **loaded** (14 parts). The task is to bring him alive:

- [ ] **Idle Animation** — Implement `CaineAnimation` class mirroring the `KingerAnimation` pattern. Start with a hover/breathe cycle and jaw flap.
- [ ] **Enable `caine.draw()`** — Uncomment in `CNAWorld::draw()`. Position Caine at a fixed boss spawn point (e.g., `posX=0, posZ=−120`).
- [ ] **Boss Health Bar** — Add `currentHealth`/`maxHealth` to `Caine`. Draw a 2D health bar in `drawCrosshair()`-style ortho overlay.
- [ ] **Phase 1 Attack — Gloink Spawn** — Implement a spawn manager in `Gloinks` (or a new `GloinkSpawner` class) that places Gloink instances at randomised arena positions on a timer. Enable `gloinks.draw()` in `CNAWorld::draw()`.
- [ ] **Bullet–Caine Collision** — In `updateSkillState()`, add a sphere–AABB check between `bulletPosX/Y/Z` and Caine's bounding box. On hit, decrement `caine.currentHealth`.
- [ ] **Kinger Damage** — Add a proximity or projectile check for Gloink–Kinger contact to decrement `kinger.currentHealth`. Wire the HUD health display.

---

### 🔲 Phase 2 — Polish & Audio
- [ ] Enable `audioManager.playBackgroundMusic()` for the arena BGM.
- [ ] Add a win/lose state check (Caine HP ≤ 0 → win screen; Kinger HP ≤ 0 → game over).
- [ ] Tune `butterflyCharges`, heal amount, and ammo count for balance.
- [ ] Add Caine death animation.
