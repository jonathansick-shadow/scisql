/*
    Copyright (C) 2011 Serge Monkewitz

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License v3 as published
    by the Free Software Foundation, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
    A copy of the LGPLv3 is available at <http://www.gnu.org/licenses/>.

    Authors:
        - Serge Monkewitz, IPAC/Caltech

    Work on this project has been sponsored by LSST and SLAC/DOE.
*/

/**
<udf name="fluxToAbMagSigma" return_type="DOUBLE PRECISION" section="photometry">
    <desc>
        Converts a cailbrated (AB) flux error to an AB magnitude error.
    </desc>
    <args>
        <arg name="flux" type="DOUBLE PRECISION" units="erg/cm&lt;sup&gt;2&lt;/sup&gt;/sec/Hz">
            Calibrated (AB) flux.
        </arg>
        <arg name="fluxSigma" type="DOUBLE PRECISION" units="erg/cm&lt;sup&gt;2&lt;/sup&gt;/sec/Hz">
            Calibrated (AB) flux error.
        </arg>
    </args>
    <notes>
        <note>
            All arguments must be convertible to type DOUBLE PRECISION.
        </note>
        <note>
            If any argument is NULL, NaN, or +/-Inf, NULL is returned.
        </note>
        <note>
            If the flux argument is zero, NULL is returned.
        </note>
    </notes>
    <example>
        SELECT fluxToAbMagSigma(rFlux_PS, rFlux_PS_Sigma)
            FROM Object
            WHERE rFlux_PS IS NOT NULL and rFlux_PS_Sigma IS NOT NULL
            LIMIT 10;
    </example>
</udf>
*/

#include <stdio.h>

#include "mysql.h"

#include "photometry.h"

#ifdef __cplusplus
extern "C" {
#endif


SCISQL_API my_bool fluxToAbMagSigma_init(UDF_INIT *initid,
                                         UDF_ARGS *args,
                                         char *message)
{
    if (args->arg_count != 2) {
        snprintf(message, MYSQL_ERRMSG_SIZE,
                 "fluxToAbMagSigma() expects exactly 2 arguments");
        return 1;
    }
    args->arg_type[0] = REAL_RESULT;
    args->arg_type[1] = REAL_RESULT;
    initid->maybe_null = 1;
    initid->const_item = (args->args[0] != 0 && args->args[1] != 0);
    initid->decimals = 31;
    return 0;
}


SCISQL_API double fluxToAbMagSigma(UDF_INIT *initid SCISQL_UNUSED,
                                   UDF_ARGS *args,
                                   char *is_null,
                                   char *error SCISQL_UNUSED)
{
    if (args->args[0] == 0 || args->args[1] == 0) {
        *is_null = 1;
        return 0.0;
    }
    return scisql_flux2absigma(*((double *) args->args[0]),
                               *((double *) args->args[1]));
}


#ifdef __cplusplus
} /* extern "C" */
#endif
