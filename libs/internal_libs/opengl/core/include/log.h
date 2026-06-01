#ifndef OPENGL_CORE_LOG_H
#define OPENGL_CORE_LOG_H

// Set to 1 to enable noisy "X created" / "X initialized" / "Brick destroyed!"
// style diagnostic logs. Default is 0 so production runs stay quiet. Errors
// always go to std::cerr regardless of this flag.
#ifndef OPENGL_VERBOSE_LOG
#define OPENGL_VERBOSE_LOG 0
#endif

#endif  // OPENGL_CORE_LOG_H
