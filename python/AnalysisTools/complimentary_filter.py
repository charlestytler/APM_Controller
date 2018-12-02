accel_mag = np.zeros(len(t))
acc_phi = np.zeros(len(t))
acc_phi2 = np.zeros(len(t))
acc_the = np.zeros(len(t))
phi_filt1 = np.zeros(len(t))
phi_filt2 = np.zeros(len(t))
the_filt = np.zeros(len(t))
the_filt2 = np.zeros(len(t))

balt = np.zeros(len(t))
ialt = np.zeros(len(t))
alt_filt = np.zeros(len(t))

u_b = np.zeros(len(t))

accel_z_zero_offset = np.mean(az[0:int(2/.02)])

for idx in np.arange(0, len(t)):
    
    accel_mag[idx] = np.sqrt(ax[idx]**2 + ay[idx]**2 + az[idx]**2)
    acc_phi[idx] = np.arctan2(-ay[idx], -az[idx])
    acc_phi2[idx] = np.arctan2(-ay[idx], -az[idx] +0.24)
    acc_the[idx] = np.arctan2(ax[idx], np.sqrt(ay[idx]**2 + az[idx]**2))
    
    
        
    if idx > 0:
        dt = t[idx] - t[idx - 1]
        ialt[idx] = ialt[idx - 1] + az[idx] * 0.5 * dt**2
        alt_filt[idx] = alt_filt[idx-1] + 0.5*(az[idx] * 0.5 * dt**2) + 0.5*balt[idx]
        
        thedot = q[idx]*np.cos(phi_filt1[idx]) - r[idx]*np.sin(phi_filt1[idx])
        the_filt[idx] = the_filt[idx - 1] + thedot * dt
        the_filt2[idx] = 0.98*(the_filt2[idx-1] + thedot * dt) + 0.02*acc_the[idx]
        
        phidot = p[idx] + (q[idx]*np.sin(phi_filt1[idx]) + r[idx]*np.cos(phi_filt1[idx]))*np.tan(the_filt[idx])
        phi_filt1[idx] = phi_filt1[idx-1] + phidot * dt
        phi_filt2[idx] = 0.98*(phi_filt2[idx-1] + phidot * dt) + 0.02*acc_phi[idx]
        
        u_b[idx] = u_b[idx-1] + ax[idx-1] * dt