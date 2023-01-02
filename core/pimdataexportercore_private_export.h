/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "pimdataexporter_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef PIMDATAEXPORTER_TESTS_EXPORT
#define PIMDATAEXPORTER_TESTS_EXPORT PIMDATAEXPORTER_EXPORT
#endif
#else /* not compiling tests */
#define PIMDATAEXPORTER_TESTS_EXPORT
#endif
