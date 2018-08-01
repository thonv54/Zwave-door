/*******************************************************************************
 *
 * 
 * 
 * 
 *
 *
 * Description:
 *
 * Author: 
 *
 * Last Changed By:  $
 * Revision:         $ 
 * Last Changed:     $ 
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "mcg.h"
#include "sim.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define MCG_C1                      ((MCG)->C1)
#define MCG_C2                      ((MCG)->C2)
#define MCG_C3                      ((MCG)->C3)
#define MCG_C4                      ((MCG)->C4)
#define MCG_C5                      ((MCG)->C5)
#define MCG_C6                      ((MCG)->C6)
#define MCG_C7                      ((MCG)->C7)
#define MCG_C8                      ((MCG)->C8)
#define MCG_C9                      ((MCG)->C9)
#define MCG_C10                     ((MCG)->C10)
#define MCG_S                       ((MCG)->S)
#define MCG_SC                      ((MCG)->SC)
#define MCG_ATCVH                   ((MCG)->ATCVH)
#define MCG_ATCVL                   ((MCG)->ATCVL)
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   MCG_ClkMonitor
 * @brief  This function simply enables or disables the OSC 0 clock monitor.
 *         This is achieved by setting or clearing the MCG_C6[CME] bit. It is 
 *         recommended to enable this monitor in external clock modes (FEE, FBE,  
 *         BLPE, PBE and PEE.
 *         It MUST be disabled in all other modes or a reset may be generated.
 *         It must also be disabled if it is desired to enter VLPR from BLPE mode.
 *
 * @param  None
 * @retval None
 */
void
MCG_ClkMonitor(
    FunctionalState newState
) {         
    if (newState) {
        MCG_C6 |= MCG_C6_CME0_MASK;   
    } 
    else {
        MCG_C6 &= ~MCG_C6_CME0_MASK;
    }
}

/**
 * @func   CLOCK_SetFastIrcDivClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SetFastIrcDivClock(
    BYTE outDiv
) {
    MCG_SC &= ~MCG_SC_FCRDIV_MASK;
    MCG_SC |=  MCG_SC_FCRDIV(outDiv);
}

/***************************  Mode and Registers  ******************************

          Mode   |   CLKST    |   IREFST   |   PLLST   |      LP
        ____________________________________________________________________

          FEI    |  00(FLL)   |   1(INT)   |   0(FLL)  |      X
        ____________________________________________________________________

          FEE    |  00(FLL)   |   0(EXT)   |   0(FLL)  |      X
        ____________________________________________________________________

          FBE    |  10(EXT)   |   0(EXT)   |   0(FLL)  |   0(NORMAL)
        ____________________________________________________________________

          FBI    |  01(INT)   |   1(INT)   |   0(FLL)  |   0(NORMAL)
        ____________________________________________________________________

          BLPI   |  01(INT)   |   1(INT)   |   0(FLL)  |   1(LOW POWER)
        ____________________________________________________________________

          BLPE   |  10(EXT)   |   0(EXT)   |     X     |   1(LOW POWER)
        ____________________________________________________________________

          PEE    |  11(PLL)   |   0(EXT)   |   1(PLL)  |      X
        ____________________________________________________________________

          PBE    |  10(EXT)   |   0(EXT)   |   1(PLL)  |   O(NORMAL)

*******************************************************************************/
/**
 * @func   MCG_GetMode
 * @brief  None
 * @param  None
 * @retval MCG Clock Mode
 */
mcgModes 
MCG_GetMode(void) {
    if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) &&       // check CLKS mux has selcted FLL output
        (MCG_S & MCG_S_IREFST_MASK) &&                                      // check FLL ref is internal ref clk
        (!(MCG_S & MCG_S_PLLST_MASK)))                                      // check PLLS mux has selected FLL
    {
        return McgModeFEI;                                                  // return FEI code
    }
    else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) &&  // check CLKS mux has selcted PLL output
             (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
             (MCG_S & MCG_S_PLLST_MASK))                                    // check PLLS mux has selected PLL 
    {
        return McgModePEE;                                                  // return PEE code
    }
    else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) &&  // check CLKS mux has selcted external reference
             (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
             (MCG_S & MCG_S_PLLST_MASK) &&                                  // check PLLS mux has selected PLL
             (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check MCG_C2[LP] bit is not set
    {
        return McgModePBE;                                                  // return PBE code
    }
    else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) &&  // check CLKS mux has selcted external reference
             (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
             (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
             (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check MCG_C2[LP] bit is not set   
    {
        return McgModeFBE;                                                  // return FBE code
    }
    else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) &&  // check CLKS mux has selcted external reference
             (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
             (MCG_C2 & MCG_C2_LP_MASK))                                     // check MCG_C2[LP] bit is set   
    {
        return McgModeBLPE;                                                 // return BLPE code
    }
    else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) &&  // check CLKS mux has selcted int ref clk
             (MCG_S & MCG_S_IREFST_MASK) &&                                 // check FLL ref is internal ref clk
             (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
             (MCG_C2 & MCG_C2_LP_MASK))                                     // check LP bit is set
    {
        return McgModeBLPI;                                                 // return BLPI code
    }
    else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) &&  // check CLKS mux has selcted int ref clk
             (MCG_S & MCG_S_IREFST_MASK) &&                                 // check FLL ref is internal ref clk
             (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
             (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check LP bit is clear
    {  
        return McgModeFBI;                                                  // return FBI code 
    }
    else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) &&  // check CLKS mux has selcted FLL
             (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
             (!(MCG_S & MCG_S_PLLST_MASK)))                                 // check PLLS mux has selected FLL
    {
        return McgModeFEE;                                                  // return FEE code
    }
    else {
        return McgModeError;                                                // error condition
    }
} 

/**  
 * @func   MCG_FEI2FBI
 * @brief  Mode transition: FBI to FEI mode
 * @param  None
 * @retval
 */
DWORD
MCG_FEI2FBI(
    DWORD irc_freq,
    BYTE irc_select
) {
    unsigned char temp_reg;
    unsigned char fcrdiv_val;
    short i;
  
    if (MCG_GetMode() != McgModeFEI) {
        return 0x1;
    } 

    // Check that the irc frequency matches the selected IRC 
    if (!(irc_select))
    {    
        if ((irc_freq < 31250) || (irc_freq > 39063)) {return 0x31;}
    }
    else
    {
        if ((irc_freq < 3000000) || (irc_freq > 5000000)) {return 0x32;} // Fast IRC freq
    }
  
    // Select the desired IRC
    if (irc_select)
    {
        MCG_C2 |= MCG_C2_IRCS_MASK; // select fast IRCS
    }
    else
    {
        MCG_C2 &= ~MCG_C2_IRCS_MASK; // select slow IRCS
    }
  
    // Change the CLKS mux to select the IRC as the MCGOUT
    temp_reg = MCG_C1;                 //
    temp_reg &= ~MCG_C1_CLKS_MASK;     // clear CLKS
    temp_reg |= MCG_C1_CLKS(1);        // select IRC as the MCG clock sourse
    MCG_C1 = temp_reg;                 //

    // Wait until internal reference switches to requested irc.
    if (!(irc_select))
    {
        for (i = 0 ; i < 2000 ; i++) {
            if (!(MCG_S & MCG_S_IRCST_MASK)) break; // jump out early if IRCST clears before loop finishes
        }

        if (MCG_S & MCG_S_IRCST_MASK) return 0x13; // check bit is really clear and return with error if set
    }
    else
    {
        for (i = 0 ; i < 2000 ; i++) {
            if (MCG_S & MCG_S_IRCST_MASK) break; // jump out early if IRCST sets before loop finishes
        }

        if (!(MCG_S & MCG_S_IRCST_MASK)) return 0x14; // check bit is really set and return with error if not set
    }
  
    // Wait for clock status bits to update
    for (i = 0 ; i < 2000 ; i++)
    {
        if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) break; // jump out early if CLKST shows IRC slected before loop finishes
    }
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1) return 0x19; // check IRC is really selected and return with error if not
  
    // Now in FBI mode
    if (irc_select)
    {
        fcrdiv_val = (1 << ((MCG_SC & MCG_SC_FCRDIV_MASK) >> MCG_SC_FCRDIV_SHIFT)); // calculate the fast IRC divder factor
        return (irc_freq / fcrdiv_val); // MCGOUT frequency equals fast IRC frequency divided by 2
    }
    else
    {
        return irc_freq; // MCGOUT frequency equals slow IRC frequency
    }   
} 

/**
 * @func   MCG_FBI2BLPI
 * @brief  Mode transition: FBI to BLPI mode
 *         This function transitions the MCG from FBI mode to BLPI mode. This is
 *         achieved by setting the MCG_C2[LP] bit. There is no status bit to 
 *         check so 0 is always returned if the function was called with the MCG
 *         in FBI mode.
 * @param  irc_freq - internal reference clock frequency
 *         ircs_select - 0 if slow irc, 1 if fast irc
 * @retval MCGOUT frequency or error code 0x13
 */
int 
MCG_FBI2BLPI(
    int irc_freq, 
    unsigned char irc_select
) {
    unsigned char fcrdiv_val;
    // check if in FBI mode
    if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
        (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
        (!(MCG_S & MCG_S_PLLST_MASK)) &&                                // check PLLS mux has selected FLL
        (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check LP bit is clear
    {  
        return 0x3;                                                     // MCG not in correct mode return fail code 
    }

    MCG_C2 |= MCG_C2_LP_MASK;                                           // Set LP bit to disable the FLL and enter BLPI
  
    // Now in BLPI
    if (irc_select) {
        fcrdiv_val = (1 << ((MCG_SC & MCG_SC_FCRDIV_MASK) >> MCG_SC_FCRDIV_SHIFT)); // calculate the fast IRC divder factor
        return (irc_freq / fcrdiv_val);                                 // MCGOUT frequency equals fast IRC frequency divided by 2
    }
    else {
        return irc_freq;                                                // MCGOUT frequency equals slow IRC frequency
    }   
}
