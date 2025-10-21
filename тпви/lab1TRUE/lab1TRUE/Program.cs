using lab1TRUE.DbContext;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using Microsoft.OpenApi.Models;
using AuthenticateResults;
namespace lab1TRUE
{
    public class Program
    {
        public static async Task Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            builder.Services.AddControllersWithViews();
            builder.Services.AddControllers();

            builder.Services.AddDbContext<AppDbContext>(options =>
            {
                options.UseSqlite("Data Source=lab1.db");
            });

            builder.Services.AddIdentity<IdentityUser, IdentityRole>(options =>
            {
                options.Password.RequireDigit = false;
                options.Password.RequireLowercase = false;
                options.Password.RequireUppercase = false;
                options.Password.RequiredLength = 4;
                options.Password.RequireNonAlphanumeric = false;
            })
            .AddEntityFrameworkStores<AppDbContext>()
            .AddDefaultTokenProviders();

            builder.Services.AddScoped<Authenticate>();

            builder.Services.AddAuthorization(options =>
            {
                options.AddPolicy("RequireReader", policy => policy.RequireRole("READER"));
                options.AddPolicy("RequireWriter", policy => policy.RequireRole("writer"));
            });

            builder.Services.AddEndpointsApiExplorer();
            builder.Services.AddSwaggerGen();

            builder.Services.AddTransient<BSTU.Results.Collection.Results>(sp =>
            {
                var filepath = Directory.GetCurrentDirectory()+ "\\Results.json";
                return new BSTU.Results.Collection.Results(filepath);
            });

            builder.Services.AddControllers();
            var app = builder.Build();



            // Configure the HTTP request pipeline.
            if (app.Environment.IsDevelopment())
            {
                app.UseSwagger();
                app.UseSwaggerUI(c =>
                {
                    c.SwaggerEndpoint("/swagger/v1/swagger.json", "v1");
                });
                
            }
            else
            {
                app.UseExceptionHandler("/Home/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }
            using (var scope = app.Services.CreateScope())
            {
                var services = scope.ServiceProvider;
                var roles = services.GetRequiredService<RoleManager<IdentityRole>>();
                var users = services.GetRequiredService<UserManager<IdentityUser>>();

                if(!await roles.RoleExistsAsync("READER"))
                {
                    IdentityRole role = new IdentityRole("READER");
                    await roles.CreateAsync(role);
                }
                if (!await roles.RoleExistsAsync("writer"))
                {
                    IdentityRole role = new IdentityRole("writer");
                    await roles.CreateAsync(role);
                }

                var readerUser = await users.FindByNameAsync("reader"); 
                if(readerUser == null)
                {
                    readerUser = new IdentityUser("reader");
                    await users.CreateAsync(readerUser,"12345");
                    await users.AddToRoleAsync(readerUser, "READER");
                }

                var writerUser = await users.FindByNameAsync("writer");
                if(writerUser == null)
                {
                    writerUser = new IdentityUser("writer");
                    await users.CreateAsync(writerUser,"12345");
                    await users.AddToRoleAsync(writerUser, "writer");
                }

            }

            app.UseHttpsRedirection();
            app.UseStaticFiles();

            app.UseRouting();

            app.UseAuthentication();
            app.UseAuthorization();


            
            app.MapControllers();
            app.MapControllerRoute(
                name: "default",
                pattern: "{controller=Home}/{action=Index}/{id?}");

            app.Run();
        }
    }
}
