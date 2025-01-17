y = var('y')
from math import pi

def function(phi_0, phi_x, phi_y, phi_xy,
             f_sin_x, f_cos_x, f_sin_y, f_cos_y, f_sin_xy, f_cos_xy,
             alpha_x, alpha_y, alpha_xy):

    f_0 = phi_0
    f_x = phi_x*(f_sin_x*sin(alpha_x*x) + f_cos_x*cos(alpha_x*x))
    f_y = phi_y*(f_sin_y*sin(alpha_y*y) + f_cos_y*cos(alpha_y*y))
    f_xy = phi_xy*(f_sin_xy*sin(alpha_xy*x*y/pi) + f_cos_xy*cos(alpha_xy*x*y/pi))
    f = f_0 + f_x + f_y + f_xy
    return f

# velocity
u = function(3.0, 1.0, 0.6, 0.0,
             0.0, 1.0, 1.0, 0.0, 1.0, 0.0,
             1.0, 1.0, 1.0)
v = integral(-diff(u,x),y)  # divergence free

# molecular viscosity
nu = 1.0

# distance to wall
y_wall = y/10.0

# Smagorinsky model
S_xx = diff(u,x)
S_xy = 0.5*(diff(u,y) + diff(v,x))
S_yy = diff(v,y)
S_yx = 0.5*(diff(u,y) + diff(v,x))
S_norm = sqrt(2*(S_xx**2 + S_xy**2 + S_yx**2 + S_yy**2))

tau_norm = sqrt(4*(S_xx**2 + S_xy**2 + S_yx**2 + S_yy**2))
shear = nu*tau_norm
u_T = shear**0.5
y_plus = y_wall*u_T/nu
A = 25.0

nu_T = (1-e**(-y_plus/A))**2.0 * S_norm
nu = nu+nu_T

tau_xx = 2*nu*diff(u,x)
tau_xy = nu*(diff(u,y) + diff(v,x))
tau_yy = 2*nu*diff(v,y)
tau_yx = nu*(diff(u,y) + diff(v,x))

# Helmholtz problem
Su = - (diff(tau_xx, x) + diff(tau_xy, y)) + u
Sv = - (diff(tau_yx, x) + diff(tau_yy, y)) + v

print 'from math import sin, cos, tanh, pi, sqrt, exp'
print ''
print 'def y_wall(X):'
print '    return', str(y_wall).replace('x', 'X[0]').replace('y', 'X[1]').replace('e^', 'exp').replace('^', '**').replace('000000000000', '')
print ''
print 'def nu_T(X):'
print '    return', str(nu_T).replace('x', 'X[0]').replace('y', 'X[1]').replace('e^', 'exp').replace('^', '**').replace('000000000000', '')
print ''
print 'def u(X):'
print '    return', str(u).replace('x', 'X[0]').replace('y', 'X[1]').replace('e^', 'exp').replace('^', '**').replace('000000000000', '')
print ''
print 'def v(X):'
print '    return', str(v).replace('x', 'X[0]').replace('y', 'X[1]').replace('e^', 'exp').replace('^', '**').replace('000000000000', '')
print ''
print 'def forcing_u(X):'
print '    return', str(Su).replace('x', 'X[0]').replace('y', 'X[1]').replace('e^', 'exp').replace('^', '**').replace('000000000000', '')
print ''
print 'def forcing_v(X):'
print '    return', str(Sv).replace('x', 'X[0]').replace('y', 'X[1]').replace('e^', 'exp').replace('^', '**').replace('000000000000', '')
print ''
print 'def U(X):'
print '   return [u(X), v(X)]'
print ''
print 'def forcing_U(X):'
print '   return [forcing_u(X), forcing_v(X)]'
print ''
