#ifndef LIBFTPP_HPP
#define LIBFTPP_HPP

// Data Structures
#include "src/data_structures/data_buffer/data_buffer.hpp"
#include "src/data_structures/data_structure.hpp"
#include "src/data_structures/pool/pool.hpp"

// Design Patterns
#include "src/design_patterns/design_patterns.hpp"
#include "src/design_patterns/memento/memento.hpp"
#include "src/design_patterns/observer/observer.hpp"
#include "src/design_patterns/singleton/singleton.hpp"
#include "src/design_patterns/state_machine/state_machine.hpp"

// Network
#include "src/network/client/client.hpp"
#include "src/network/message/message.hpp"
#include "src/network/ring_buffer/ring_buffer.hpp"
#include "src/network/server/server.hpp"

// Threading
#include "src/thread/persistent_worker/persistent_worker.hpp"
#include "src/thread/thread/thread.hpp"
#include "src/thread/thread_safe_iostream/thread_safe_iostream.hpp"
#include "src/thread/thread_safe_queue/thread_safe_queue.hpp"
#include "src/thread/threading.hpp"
#include "src/thread/worker_pool/worker_pool.hpp"

// Mathematic
#include "src/mathematics/IVector2/ivector2.hpp"
#include "src/mathematics/IVector3/ivector3.hpp"
#include "src/mathematics/perlin_noise/perlin_noise.hpp"
#include "src/mathematics/random_2D_coordinate_generator/random_2D_coordinate_generator.hpp"

#endif