pIndex = i + j * (nx + 2);
nIndex = i + (j + 1) * (nx + 2);
sIndex = i + (j - 1) * (nx + 2);
wIndex = i - 1 + j * (nx + 2);
eIndex = i + 1 + j * (nx + 2);

// compute the convection-diffusion coefficients (Scaled)
Fs = (U[1][i][j-1] + U[1][i][j]) / 2.0 * dx;
Fn = (U[1][i][j+1] + U[1][i][j]) / 2.0 * dx;
Fe = (U[0][i+1][j] + U[0][i][j]) / 2.0 * dy;
Fw = (U[0][i-1][j] + U[0][i][j]) / 2.0 * dy;
Ds = (nu[i][j] + nu[i][j-1]) / 2.0 / dy * dx;
Dn = (nu[i][j] + nu[i][j+1]) / 2.0 / dy * dx;
De = (nu[i][j] + nu[i+1][j]) / 2.0 / dx * dy;
Dw = (nu[i][j] + nu[i-1][j]) / 2.0 / dx * dy;
as = std::max(std::max(Fs, Ds + Fs / 2.0), 0.0);
an = std::max(std::max(-Fn, Dn - Fn / 2.0), 0.0);
ae = std::max(std::max(-Fe, De - Fe / 2.0), 0.0);
aw = std::max(std::max(Fw, Dw + Fw / 2.0), 0.0);
ap = Fe - Fw + Fn - Fs + as + an + ae + aw;

// compute the residual
y[pIndex] = vol * S[i][j] + as * x[sIndex] + an * x[nIndex] + 
ae * x[eIndex] + aw * x[wIndex] - ap * x[pIndex];