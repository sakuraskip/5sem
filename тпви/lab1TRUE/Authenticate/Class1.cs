using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
namespace AuthenticateResults
{
    public class Authenticate
    {
        private UserManager<IdentityUser> _userManager;
        private SignInManager<IdentityUser> _signInManager;

        public Authenticate(UserManager<IdentityUser> userManager, SignInManager<IdentityUser> signInManager)
        {
            _userManager = userManager;
            _signInManager = signInManager;
        }
        public async Task<SignInResult> SignInAsync(string username, string password)
        {
            var user = await _userManager.FindByNameAsync(username);
            if (user != null)
            {
                return await _signInManager.PasswordSignInAsync(user, password, false, false);
            }
            return SignInResult.Failed;
        }
        public async Task SignOut()
        {
            await _signInManager.SignOutAsync();
        }
    }
}
